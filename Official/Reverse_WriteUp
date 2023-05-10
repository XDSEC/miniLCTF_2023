题目使用一个LLVM Pass处理了作为ir的main.bc，加密逻辑使用与非虚拟机，通过`llvm::Value::getName()`获取ir里的函数名作为opcode，一个函数名加密一位flag，加密逻辑为前半段异或后半段，后半段异或0xff。
想要知道此函数获取了什么数据作为opcode，可以查询LLVM的文档或下载相应版本的LLVM进行调试。
至于如何从main.bc中获取函数名，可以将main.bc编译后获取符号表，按以下方式操作：

```
$ llc main.bc -o main.s
$ clang main.s
$ readelf a.out -s
```

或者直接去看main.bc的hex，在偏移为0x1e9e的地方也可获得函数名。
或者也可以调试获取opcode。
