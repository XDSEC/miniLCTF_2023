# miniL CTF 2023 partial WriteUps by Koito Coco

## Crypto WriteUp - giveaway

***坏了，给我这非预期一搞，真白给了***
***感谢polar老师的指导，我是真的不会sagemath呜呜呜，我菜死了喵***

首先有个`proof_of_work`，有手就行，直接写个小工具爆破即可

```python
#!venv/bin/python

from itertools import product
from hashlib import sha256


def main():
    known = (known_[5:] if (known_ := input("known part: "))[:5] == "XXXX+" else known_)
    sha = input("sha256: ")

    for a, b, c, d in product("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", repeat=4):
        if sha256((a + b + c + d + known).encode()).hexdigest() == sha:
            print(a + b + c + d)
            return
    print("not found")


if __name__ == "__main__":
    main()
```

然后进入正题，先贴一些关键的题目代码，代码不是完整的，完整的请看题目的GitHub仓库

```python
def dec2blist(decimal, length):
    """
    Converts an integer to a binary list of a specified length, filling zeros on the left if necessary.
    """
    bitlist = []
    while decimal > 0:
        bit = decimal % 2
        bitlist.append(bit)
        decimal //= 2
    bitlist.reverse()
    if len(bitlist) < length:
        bitlist = [0] * (length - len(bitlist)) + bitlist
    return bitlist
```
这个`dec2blist`还是好理解的，就是把一个整数转换成一个0和1的列表，如果长度小于给定的长度，在前端补0

```python
def bake(list1,list2):
    return reduce((lambda x,y: x ^ y) , list(map(lambda x, y: x and y, list1, list2)) )
```
这个`bake`看起来挺短，但是还是得琢磨一下的，表面上看是两个列表逐一做and运算，然后通过一个xor做reduce，我的理解是，假定两个参数是长度相等的列表，且每一个元素的值都是0或者1，则可以把and运算看成`list1`和`list2`两个n维向量逐项相乘，得到的每一项也都是0或者1，因此xor运算就可以看做把相乘得到各项相加，然后取mod2。也就是`bake`是求两个向量的数量积，然后取mod 2，换句话说，是在mod2有限域下的向量乘法

```python
def send_a_chocolate(self):
    assert len(bin(bytes_to_long(message)))-2==511
    dough=bytes_to_long(message)
    chocolate_jam=random.getrandbits(512)
    cookie=bake(dec2blist(dough, 512), dec2blist(chocolate_jam,512))
    self.send(f"[+] The lucky sticker reads ({cookie},{hex(chocolate_jam)}). Yummy!\n".encode())
```
这个也好理解，就是把flag和一个随机的jam通过bake得到一个result，将result和jam一起返回

虽然出题人的本意是限制获取不超过508组jam和result的，但是这是静态flag，而且也没加salt（
那我多次请求不就好了吗（~~polar: 草草草草，黑客！~~

于是我就获得了512组jam和result

每个jam是一个长512的行向量，构成了一个512x512的系数方阵*A*，要求的flag构成了未知列向量*x*，result构成了列向量*u*，有方程 *A \* x* == *u*

方程放到sage里面，求解就ok了

```python
from xxx import A, x, u # 这个得自己想办法了，太长了把我worksheet都搞卡了（

# 这是sagemath
R = IntegerModRing(2)
A = Matrix(R,A)
v = vector(R,v)
x = A.solve_right(v)
```
得到的还是一个0和1的列表，再写个小脚本转换一下

```python
from Crypto.Util.number import long_to_bytes

print(long_to_bytes(int("".join(str(bit) for bit in x),2)))
```

done

## Web WriteUp - Signin

先贴题目shell.php (刚打开的时候是一个**特别土的字符雨**以及**不明所以的面具人**和**极度中二的几行话**，f12查看页面源码可以看到注释中提示有shell.php)

```php
<?php

error_reporting(0);
show_source(__FILE__);

$a = $_GET["a"];
$b = $_GET["b"];
$c = $_GET["c"];
$d = $_GET["d"];
$e = $_GET["e"];
$f = $_GET["f"];
$g = $_GET["g"];

if(preg_match("/Error|ArrayIterator|SplFileObject/i", $a)) {
    die("你今天rce不了一点");
}


if(preg_match("/php/i", $b)) {
 die("别上🐎，想捣蛋啊哥们？");
}

if(preg_match("/Error|ArrayIterator/i", $c)) {
 die("你今天rce不了一点");
}


$class = new $a($b);
$str1 = substr($class->$c(),$d,$e);
$str2 = substr($class->$c(),$f,$g);
$str1($str2);

//flag.php
```

经过简单分析，可以知道这段代码是从查询字符串里面读出参数~~废话~~，然后用`$b`为**参数**，初始化了一个**名称**为`$a`的**内部类**，再从这个对象实例中的**名称**为`$c`的方法的返回**结果**中，**提取了两个子字符串**

首先可以排除`$a`和`$c`这两个参数，因为一个是内部类，一个是方法名，肯定不是可自由控制的，那就只能是`$b`为自由控制的了。

其中这里对`$a`进行的一个过滤反而给了提示，简单查查就可以发现`Exception`这个类，接受一个字符串参数作为错误消息，并且可以通过`getMessage()`方法来获取这个消息，这不就秒了吗（

你先别急，这还有对`$b`的过滤呢，最末尾已经提示了"flag.php"，但是php又在过滤范围内，怎么搞？别忘了`system()`函数调的是一个shell，可以用一些sh的语法，比如`ls` -> `ls | grep flag` -> `cat $(ls | grep flag)`，这不就读到了吗（

你还是别急，这么做之后你会发现，怎么没有回显？？？？？？

我猜测是混进源码被解释器执行了，无所谓，只要能提出flag就行了，别的行爱怎么样怎么样，再来一个grep！`cat $(ls | grep flag) | grep {` 直接筛选有`{`的行，最后回显有且仅有flag的内容

上payload `http://127.0.0.1/shell.php?a=Exception&b=systemcat $(ls . | grep flag) | grep {&c=getMessage&d=0&e=6&f=6&g=114`

## Misc WriteUp - PyCalc

emmm，这是个**misc**，也没有附件，只有一个~~wsrx地址~~，用nc连上，提示这是一个基于python的计算器，随便输点发现只要有`A-z`的字符出现，马上就会报hacker并切断链接。

显然，这是一个pyjail的逃逸。

既然`A-z`都不能用，那就得想办法替代`A-z`，上课摸鱼一顿搜，搜到了[PEP - 3131](https://peps.python.org/pep-3131/)，这个规范提出了可以用一部分等效的unicode字符来作为python identifier的一部分，找到对应的unicode字符表，可以写一段简单的代码进行转换

```python
"".join([(chr(ord(x)-ord("a")+ord("\U0001d44e")) if ord(x)>= ord("A") and ord(x)<=ord("z") else x) for x in "eval(input())"])
```

转换得到结果`𝑒𝑣𝑎𝑙(𝑖𝑛𝑝𝑢𝑡())`，丢到解释器里面执行，发现和`eval(input())`的行为一模一样，直接把这一行喂给题目环境，再输入指令

```python
__import__("os").system("cat flag")
```

over

## Misc WriteUp - evase64

先看题，题目很短

```python
import ast
import base64

input_data = input(">>> ")

parser_tree = ast.parse(input_data, mode = 'eval')
assert not any(isinstance(node, ast.Call) for node in ast.walk(parser_tree))
del ast

eval_result = eval(compile(parser_tree, filename = '', mode = 'eval'))
decode_base64 = int.from_bytes(base64.b64decode(input_data), byteorder = 'little')
del base64

if eval_result == decode_base64:
	flag = open("flag").read().strip()
	print(flag)
```

主要做的两件事情就是，先用ast验证是否存在函数调用，如果存在，则直接assert报错
然后分别用eval和base64解码得到两个值，进行比较，如果为`True`则返回flag

ast的函数调用限制基本上是绕不开了，而且这里用的是eval模式，也不能用`=`运算，所以入手点只能是base64的一些特性

看base64库的`b64decode`默认参数，输入的是string或者bytes 然后有一个验证选项

```python
def b64decode(s, altchars=None, validate=False):
    """Decode the Base64 encoded bytes-like object or ASCII string s.

    Optional altchars must be a bytes-like object or ASCII string of length 2
    which specifies the alternative alphabet used instead of the '+' and '/'
    characters.

    The result is returned as a bytes object.  A binascii.Error is raised if
    s is incorrectly padded.

    If validate is False (the default), characters that are neither in the
    normal base-64 alphabet nor the alternative alphabet are discarded prior
    to the padding check.  If validate is True, these non-alphabet characters
    in the input result in a binascii.Error.
    """
    s = _bytes_from_decode_data(s)
    if altchars is not None:
        altchars = _bytes_from_decode_data(altchars)
        assert len(altchars) == 2, repr(altchars)
        s = s.translate(bytes.maketrans(altchars, b'+/'))
    if validate and not re.fullmatch(b'[A-Za-z0-9+/]*={0,2}', s):
        raise binascii.Error('Non-base64 digit found')
    return binascii.a2b_base64(s)
```

这个验证会要求输入在`[A-z0-9+/]`之间，但是默认是关闭的 也就是忽略无效的字节，所以如果我的payload里面没有在`[A-z0-9+/]`之间的字符，那最终base64解出来是0，所以eval用这之外的字符构造一个返回值为`val`的表达式，能使得`val == 0`为`True`就可以了

在python里面还有一个小trick，就是`False == 0`，所以有了一个最短的payload（之一，目前已发现的）`[]!=[]`
