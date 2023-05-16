## 3calls

给出 libc 基地址, 仅允许三次 libc 函数调用, 但是不能传参数. 观察一下寄存器, 可以发现 rdi 是 libc 的数据段, 具有写权限. gdb 加载一下 libc 的调试符号, 可以看到是 `_IO_stdfile_1_lock`. 考虑使用 `system()` 函数, 需要向 rdi 上写一个 `"/bin/sh"`, 很容易想到 `gets()` 函数. 尝试一下, 发现 `gets()` 之后 rdi 会变, 不过是也是一个可写的地址, 这回变成了 `_IO_stdfile_0_lock`. 在调用用户指定函数之前, 程序有一个 `puts()`, 可以发现这里的两个 rdi 的 lock 分别对应了 stdout (fd = 1) 和 stdin (fd = 0), 可以猜测是某种锁. 所以可以再一次调用 `gets()`, 第二次 `gets()` 前后 rdi 都是 `_IO_stdfile_0_lock`, 所以可以输入 `"/bin/sh"`, 然后调用 `system()`.

不过, 第一个 `gets()` 内容不能随意输入, 因为它会覆盖 `_IO_stdfile_1_lock` 的内容, 如果输入不得当, 相当于 "被上锁". 第二个 `gets()` 中会去尝试获取这个锁, 导致死锁. 所以第一个 `gets()` 输入的内容必须要绕过这个点. 可以只输入一个 `"\n"`, 不会向 `_IO_stdfile_1_lock` 输入内容; 或者输入 `"\0" * 0x8`, 不破坏锁的内容.

第二个 `gets()` 输入 `"/bin/sh"` 并不能够获得 shell, 可以看到提示 `sh: 1: /bin.sh: not found`. 这是因为 `gets()` 会先获取 `_IO_stdfile_0_lock`, 结束后又会释放这个锁. 在这中间过程, 我们把他覆盖成了 `"/bin/sh"`, 释放锁的话会破坏掉它. 锁结构在 0x4 的位置有一个 `cnt` 变量, 用于计数拥有锁的进程, 释放锁的时候这个值会减 1. 于是变成了 `/bin.sh`. 所以我们需要把第二个 `'/'` (0x2f) 变为 `'0'` (0x30), 这样释放锁后, 整个就变为 `"/bin/sh"`.

## twins

程序是简单的栈溢出, 但是会启动两个程序, 对比他们的输出, 如果不一致则退出. 一般情况我们是泄漏 libc, 然后 ROP get shell. 不过由于 ASLR 的原因, 两个程序的 libc 地址会不一样, 即不能泄漏 libc. 而内存中是有 libc 地址的, 比如 .bss 上的 stdin 等. 程序没有开 PIE, 放有 libc 地址的内存已知. 而我们学过的 ret2csu 可以 call [addr], 如果能写一个 ROP 直接通过偏移计算想要的函数, 那么就可以 ret2csu 调用, 不需要泄漏. 用一些工具可以找到这样的一个 gadget: `0x000000000040115c : add dword ptr [rbp - 0x3d], ebx ; nop ; ret`. 如果可以控制 rbp 和 ebx, 那么就可以进行上述操作. 幸运的是, ret2csu 其实就有 `pop rbx; pop rbp`.

程序没开 FULL RELRO, 存在一个非预期 ret2dlresolve. (出题人没学过这个, 爬了)

## broken_machine

程序读入一段内容到 .bss 上, 只能有一个 `n`. 然后尝试在 0x1000 地址 mmap 一段 rwx 权限的空间, 用 `sprintf(s, buf)` 拷贝过去. 之后设置沙箱, call 0x1000 去执行. 但是 `mmap_min_addr` 默认 0x10000, 所以实际上申请到的地址是 0x10000, call 0x1000 会导致段错误. 程序在一开始捕捉 `SIGSEGV` 信号, 处理段错误, 输出一些提示后 `exit()` 退出.

很明显的格式化字符串漏洞. 但是 buf 不在栈上, 且只能一次 `%n`, 想任意地址写比较难. 观察一下栈, 发现栈上有残留的 `_rtld_global.dl_ns[0]._ns_loaded->link_map->l_addr` 的地址.

程序使用 `exit()` 退出的时候会调用 `_dl_fini()`, 执行如下代码:

```c
            // l = _rtld_global.dl_ns[0]._ns_loaded->link_map
            // DT_FINI_ARRAY = 26
            // DT_FINI_ARRAYSZ = 28
              if (l->l_info[DT_FINI_ARRAY] != NULL)
            {
              ElfW(Addr) *array =
                (ElfW(Addr) *) (l->l_addr
                        + l->l_info[DT_FINI_ARRAY]->d_un.d_ptr);
              unsigned int i = (l->l_info[DT_FINI_ARRAYSZ]->d_un.d_val
                        / sizeof (ElfW(Addr)));
              while (i-- > 0)
                ((fini_t) array[i]) ();
            }
```

这段代码原本是用来执行程序 .fini.array 中的函数的, 在本题 (没有开启 PIE) 的情况下, `l->l_addr = 0`, `l->l_info[DT_FINI_ARRAY]->d_un.d_ptr = .fini.array`, `l->l_info[DT_FINI_ARRAYSZ]->d_un.d_val = 8`.

可以用一次 `%n`, 把 `l->l_addr` 写上其他的值, 这样最后的 `array` 就会偏移. 我们可以让 `array` 到 bss 的某处, 在这里通过输入写上 0x10000, 最后 `exit()` -> `_dl_fini()` 就可以执行写入的 shellcode 了. 需要注意的是, `sprintf()` 遇到 `'\0'` 会停止, 编写的 shellcode 不要有 `0x00` 字节.

(如果开启了 PIE, 那么 `l->l_addr` 是程序基地址, `l->l_info[DT_FINI_ARRAY]->d_un.d_ptr` 是 `.fini.array` 的偏移量)

现在我们可以执行 shellcode 了, 但是沙箱禁用了几乎所有的 orw:

```
 line  CODE  JT   JF      K
=================================
 0000: 0x20 0x00 0x00 0x00000004  A = arch
 0001: 0x15 0x00 0x14 0xc000003e  if (A != ARCH_X86_64) goto 0022
 0002: 0x20 0x00 0x00 0x00000000  A = sys_number
 0003: 0x35 0x12 0x00 0x40000000  if (A >= 0x40000000) goto 0022
 0004: 0x15 0x11 0x00 0x0000003b  if (A == execve) goto 0022
 0005: 0x15 0x10 0x00 0x00000142  if (A == execveat) goto 0022
 0006: 0x15 0x0f 0x00 0x00000002  if (A == open) goto 0022
 0007: 0x15 0x0e 0x00 0x00000101  if (A == openat) goto 0022
 0008: 0x15 0x0d 0x00 0x00000000  if (A == read) goto 0022
 0009: 0x15 0x0c 0x00 0x00000013  if (A == readv) goto 0022
 0010: 0x15 0x0b 0x00 0x00000011  if (A == pread64) goto 0022
 0011: 0x15 0x0a 0x00 0x00000127  if (A == preadv) goto 0022
 0012: 0x15 0x09 0x00 0x00000039  if (A == fork) goto 0022
 0013: 0x15 0x08 0x00 0x0000003a  if (A == vfork) goto 0022
 0014: 0x15 0x07 0x00 0x00000029  if (A == socket) goto 0022
 0015: 0x15 0x06 0x00 0x0000002a  if (A == connect) goto 0022
 0016: 0x15 0x05 0x00 0x0000002b  if (A == accept) goto 0022
 0017: 0x15 0x04 0x00 0x0000009d  if (A == prctl) goto 0022
 0018: 0x15 0x03 0x00 0x00000065  if (A == ptrace) goto 0022
 0019: 0x15 0x02 0x00 0x00000009  if (A == mmap) goto 0022
 0020: 0x15 0x01 0x00 0x0000000a  if (A == mprotect) goto 0022
 0021: 0x06 0x00 0x00 0x7fff0000  return ALLOW
 0022: 0x06 0x00 0x00 0x00000000  return KILL
```

ubuntu 22.04 有一个新的 syscall (暂时没去查是哪个 kernel 版本引入的, ubuntu 22.04 的内核版本是 5.15), `openat2`, 它的功能是 `openat` 的超集, 所以可以用它来打开 flag 文件. read 全被禁用了也没关系, 有一个叫 `sendfile` 的系统调用, 可以直接通过文件描述符来传送.

设置沙箱的位置在 `sprintf` 之后, 存在非预期直接利用 `sprintf` 产生段错误, 绕过沙箱.

## ezbook

堆菜单题, 增改查没有删, 个数 0x100, 大小 0x1000. book 的 title 和 content 都要求唯一, 并且在 create 和 edit 时, 会创建一个线程来遍历检查. 检查每个 book 的每个字符, 但并不是完全遍历, 而是一旦发现不同, 立马返回. check 失败后会 free.

程序对每个变量都加了锁, 但是对 books 使用的是读写锁. 读写锁可以供多个线程读取, 但是只能有一个线程写, 其他线程不可读不可写. 程序在 create 时, 不管 title 或者 content 是否唯一, 先记录了 sizes 变量, 然后 check 发现不唯一后, 把 `size_cnt--`; 唯一的话再设置 `books[book_cnt++] = buf`. 这里存在一个条件竞争.

如果某个会失败的 create (假设第 i 个) 经历了很多字符比较, 用时较长, 且我们在进行比较的时间窗口内, 再 create 一个 (第 i+1 个), 此时会导致 `size_cnt++`. 当第 i 个的 check 失败时, `size_cnt--`. 但是第 i+1 个是成功的. 这样就会让第 i+1 个 book 对应的是第 i 个的 size. 如果第 i 个的 size 大于第 i+1 个的, 那么 edit 时就会溢出.

完成这个条件竞争后就是堆溢出了, 不再赘述.

远程可能会有一些网络延迟, 出题时刻意在比较字符的时候进行了一系列文件操作, 同时把 book 个数和大小放的很大, 应该是能够 race 上的. 赛时出题人 (在学校) 自测用 4 个 book, 每个 0xfff 个字节去比较可以达成.

(exp 在 leak libc 的时候用的是 0x80 先填充 tcache bin, 因为前一版题目大小限制了 0x80, 后来也懒得改了. 其实完全没有必要这样做, 可以直接 free 一个更大的堆块到 unsorted bin 然后 malloc 回来 show)
