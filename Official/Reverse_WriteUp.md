## EasyPass

题目使用一个LLVM Pass处理了作为ir的main.bc，加密逻辑使用与非虚拟机。关于此虚拟机，可以参考https://www.yunzh1jun.com/2023/05/02/d3sky/

题目通过`llvm::Value::getName()`获取ir里的函数名作为opcode，一个函数名加密一位flag，加密逻辑为前半段异或后半段，后半段异或0xff。

想要知道此函数获取了什么数据作为opcode，可以查询LLVM的文档或下载相应版本的LLVM进行调试。

至于如何从main.bc中获取函数名，可以将main.bc编译后获取符号表，按以下方式操作：

```
$ llc main.bc -o main.s
$ clang main.s
$ readelf a.out -s
```

或者直接去看main.bc的hex，在偏移为0x1e9e的地方也可获得函数名。
或者也可以调试获取opcode。

解题部分调试方法可以看[dr3的wp](https://github.com/XDSEC/miniLCTF_2023/blob/main/WriteUps/%C9%AF%C9%90%C7%9D%CA%87%CB%A5/Easypass.md)，写得很详细。
预期解最终exp：

``` python
enc = [100, 4, 101, 15, 44, 93, 57, 35, 35, 0, 22, 5, 29, 143, 147, 154, 160, 179, 147, 169, 146, 160, 175, 203, 140, 202]

for i in range(13,26):
	enc[i]^=0xff
for i in range(13):
	enc[i]^=enc[25-i]
print(bytes(enc))
```
