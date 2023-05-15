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

```python
enc = [100, 4, 101, 15, 44, 93, 57, 35, 35, 0, 22, 5, 29, 143, 147, 154, 160, 179, 147, 169, 146, 160, 175, 203, 140, 202]

for i in range(13,26):
    enc[i]^=0xff
for i in range(13):
    enc[i]^=enc[25-i]
print(bytes(enc))
```

## maze_aot

一个有向迷宫，但是预先编译（AOT）优化

### 考点

控制流，汇编代码的分析和处理，自动化，~~算法~~

### 生成

从[这里](assets/maze.zip)查看生成二进制文件的源代码。控制流的生成使用了[llvmlite](https://github.com/numba/llvmlite)库，在Linux下运行`./maze.py`即可生成`maze`可执行文件，在windows下运行时也可以生成`maze.exe`，但是它没有生成`maze.pdb`导致最终二进制没有符号信息。

### 预期解1

从汇编代码中抽象出控制流块的前后关系，然后用最短路径算法求解。这里由于`maze_walk`中的指令比较单一，直接用capstone反汇编引擎恢复控制流（看了选手的题解，可能用IDAPython或angr会更方便一点）：

```python
#!/usr/bin/env python3

from elftools.elf.elffile import ELFFile
from elftools.construct import Container
from capstone import Cs, CS_ARCH_X86, CS_MODE_64
from networkx import DiGraph, shortest_path
from Crypto.Cipher import ARC4

# 从符号获取字节数据
def getdata(elf: ELFFile, entry: Container) -> bytes:
    addr, size = entry.st_value, entry.st_size
    for section in elf.iter_sections():
        start = section.header.sh_addr
        end = start + section.header.sh_size
        if start <= addr <= end:
            assert addr + size <= end
            return section.data()[addr-start:addr-start+size]
    raise ValueError('cannot find data from 0x%x to 0x%x' % (addr, addr + size))

elf = ELFFile(open('maze', 'rb'))
symtab = elf.get_section_by_name('.symtab')

# 获取函数的偏移和数据
maze_walk = symtab.get_symbol_by_name('maze_walk')[0].entry
code = getdata(elf, maze_walk)
maze_walk_addr = maze_walk.st_value
maze_step_addr = symtab.get_symbol_by_name('maze_step')[0].entry.st_value
maze_final_addr = symtab.get_symbol_by_name('maze_final')[0].entry.st_value

# 获取加密的flag数据
flag_enc = getdata(elf, symtab.get_symbol_by_name('maze_flag')[0].entry)
flag_enc = flag_enc[:-1] # 最后一个字节为结束字符

# 使用capstone将字节数据转换为汇编
cs = Cs(CS_ARCH_X86, CS_MODE_64)

# 把控制流块提取出来
blocks, current = [], None
final_block_addr = None # 最终块的地址
for addr, _, op, opr in cs.disasm_lite(code, maze_walk_addr):
    # insn = op + (' ' + opr if opr else '')
    # print('%04x %s' % (addr, insn))

    if op == 'nop': continue # 存在nop是因为指令对齐，过滤即可
    elif op == 'call': # 可能调用maze_step和maze_final
        if int(opr, 0) == maze_step_addr:
            current = []
            blocks.append(current)
            current.append((addr, op, opr))
        elif int(opr, 0) == maze_final_addr:
            final_block_addr = addr
        else: raise ValueError('unknown function %s' % opr)
    else:
        if current is not None: # 其他指令直接插入当前块
            current.append((addr, op, opr))

# patterns = set() # 先统计基本块模式的种类
# for block in blocks:
#     pattern = ' '.join(op for _, op, _ in block)
#     patterns.add(pattern)
#     for addr, op, opr in block:
#         if opr: op += ' ' + opr
#         print('%04x %s' % (addr, op))
#     print()
# print(patterns) # > {'call test je', 'call test jne', 'call test jne jmp', 'call test je jmp'}

graph = [] # 地址，0分支，1分支
blocks.append((final_block_addr, 0, 0))
for i, block in enumerate(blocks[:-1]):
    pattern = ' '.join(op for _, op, _ in block)
    if pattern == 'call test je':
        graph.append((block[0][0], int(block[2][2], 0), blocks[i + 1][0][0]))
    elif pattern == 'call test jne':
        graph.append((block[0][0], blocks[i + 1][0][0], int(block[2][2], 0)))
    elif pattern == 'call test jne jmp':
        graph.append((block[0][0], int(block[3][2], 0), int(block[2][2], 0)))
    elif pattern == 'call test je jmp':
        graph.append((block[0][0], int(block[2][2], 0), int(block[3][2], 0)))
    else: raise ValueError('unknown pattern %r' % pattern)

# 转换成有向图
addr2idx = {node[0]: i for i, node in enumerate(graph)}
addr2idx[final_block_addr] = len(addr2idx) # 最终块
graph = list((addr2idx[node[1]], addr2idx[node[2]]) for node in graph)
# -> [(1, 5), (8, 2), (11, 3), ...]

# 求最短路径可以直接调库
G = DiGraph()
for i, node in enumerate(graph):
    G.add_edge(i, node[0])
    G.add_edge(i, node[1])
path = shortest_path(G, 0, len(graph))

key, current = 0, 0
assert len(path) == 65
for i, v in enumerate(path[1:]):
    key |= graph[current].index(v) << i
    current = v
key = key.to_bytes(8, 'little')
print('miniLctf{%s}' % ARC4.new(key).decrypt(flag_enc).decode())
```

### 预期解2

注意到程序控制流复杂但逻辑简单，而且没有太多层函数调用，我们可以利用[angr](https://github.com/angr/angr)符号执行框架来进行求解（为了避免路径爆炸，实现了一个自定义的`ExplorationTechnique`）：

```python
from typing import Any
from angr import Project, SimState, SimulationManager, ExplorationTechnique
from angr.sim_options import ZERO_FILL_UNCONSTRAINED_REGISTERS
from cle.backends.symbol import Symbol
from claripy import *

class MazeWalker(ExplorationTechnique):
    '''遍历方法：跳过已访问的代码，类似于BFS'''

    def __init__(self, maze_range: tuple[int, int], nstep_addr: int, final_addr: int) -> None:
        self._range = maze_range
        self._nstep_addr = nstep_addr
        self._final_addr = final_addr
        self._depth: dict[int, int] = {}

    def setup(self, simgr: SimulationManager) -> None:
        simgr.stashes['visited'] = []
        simgr.stashes['found'] = []
        self._depth[simgr.active[0].addr] = 0

    def current_pos(self, state: SimState) -> int:
        '''返回`current`变量的值'''
        return state.solver.eval(state.mem[self._nstep_addr].unsigned.resolved, cast_to=int)

    def inside_maze(self, state: SimState) -> bool:
        '''是否在`maze_walk`函数中'''
        return self._range[0] <= state.addr < self._range[1]

    def step(self, simgr: SimulationManager, stash: str='active', **kwargs: Any) -> SimulationManager:
        '''在`step`方法中作额外的处理'''
        simgr.step(stash=stash, **kwargs)
        def state_spiltter(states: list[SimState]) -> tuple[list[SimState], list[SimState]]:
            keep, split = [], []
            for state in states:
                if self.inside_maze(state): # 只处理函数内部
                    addr = state.addr
                    assert isinstance(addr, int)
                    current = self.current_pos(state)
                    if addr in self._depth and self._depth[addr] < current:
                        split.append(state) # 丢弃已经访问过的部分
                    else:
                        self._depth[addr] = current
                        keep.append(state)
                else:
                    keep.append(state)
            return keep, split
        simgr.split(state_spiltter, from_stash='active', to_stash='visited')
        simgr.move('active', 'found', lambda state: state.addr == self._final_addr)
        return simgr

    def complete(self, simgr: SimulationManager) -> bool:
        return bool(simgr.found)

proj = Project('maze', auto_load_libs=False, main_opts={'base_addr': 0})
key: Symbol = proj.loader.find_symbol('key')
steps: Symbol = proj.loader.find_symbol('steps')
current: Symbol = proj.loader.find_symbol('current')
maze_walk: Symbol = proj.loader.find_symbol('maze_walk')
maze_final: Symbol = proj.loader.find_symbol('maze_final')

state: SimState = proj.factory.call_state(maze_walk.rebased_addr, prototype='void f()')
state.options.add(ZERO_FILL_UNCONSTRAINED_REGISTERS)
key_value = BVS('key', 64)
state.memory.store(steps.rebased_addr, key_value)
state.memory.store(key.rebased_addr, key_value)

# 使用我们自己的`ExplorationTechnique`
tech = MazeWalker(
    (maze_walk.rebased_addr, maze_walk.rebased_addr + maze_walk.size),
    current.rebased_addr,
    maze_final.rebased_addr
)
simgr = proj.factory.simgr(state)
simgr.use_technique(tech)

# 运行直到产生 `found` stash
simgr.run()
simgr.remove_technique(tech)

# 继续运行直到输出flag
print(
    proj.factory
    .simgr(simgr.found[0])
    .run()
    .deadended[0]
    .posix.dumps(1)
)
```

### flag

> `miniLctf{YOU_AR3_$0_GOOD_4T_SOLV1NG_MAZE}`

## hacker's gift

### 如何找到主要逻辑

拿gift程序举例：

在函数列表中搜索`main`，发现一个叫`client::main::xxxx`的函数，点开里面就是主要逻辑，而client就是这个rust程序的包名。

再在函数列表中搜索`client`即可看到所有出题人自己写的函数，主要加密逻辑在encrypt函数中。

### 程序运行流程

附件提供流量包目的也是帮选手梳理思路，应该还是挺清晰的。

server程序运行在hacker X的服务器上，client是DX收到的gift。

DX闲着没事干运行这个程序时，client会首先在本地生成一个RSA密钥对，然后把公钥放在http header里，向server发出第一个getkey请求。server收到请求后，会生成一个0-255的随机数，拼接上一个固定的字符串，作为key。用client发来的公钥加密key，放在response body中返回。

client拿到加密的key，用私钥解密拿到明文key。然后遍历同路径下一个叫`dangerous_directory`的文件夹，对于每一个文件，每次读入1024位，与key的第`i%key_len`位异或，直到文件内容读取结束。然后把文件名放在request header，加密数据放到request body里，向server发出upload请求。

### 解题思路 & 脚本

搞清楚流程之后解题就很简单了。只需从流量包里把flag文件数据搞出来，再拿到密钥进行异或解密即可。

至于密钥前几位的随机数，可以看到流量包里上传的还有一个jpg文件，根据jpg文件头恢复key前几位即可。

根据选手的反馈，每次读入1024并以1024位为单位进行加密这个点不容易看出来，这块就很需要耐心。

```c
$LT$std..process..ChildStdout$u20$as$u20$std..io..Read$GT$::read::h253e3cddca3ae7b5(
          &v81,
          a2 + 1720,
          a2,
          1024LL);
```

read函数最后一个参数指明了每次读入长度为1024

最终脚本：

```rust
use std::fs;
use std::io::Read;
use std::io::Write;
fn main() {
    let key = Vec::from("221e07f177-e9e8-494d-bd46-ab791cb4c694");
    let key_len = key.len();
    let mut file_read = fs::OpenOptions::new().read(true).open("flag.enc").unwrap();
    let mut file_write = fs::OpenOptions::new().write(true).append(true).open("flag").unwrap();
    let mut buf = [0; 1024];
    let mut buf_write = Vec::new();
    while let Ok(bytes_read) = file_read.read(&mut buf) {
        if bytes_read == 0 {
            break;
        }
        for i in 0..bytes_read {
            buf[i] ^= key[i % key_len];
            buf_write.push(buf[i]);
        }
        file_write.write(&buf_write).unwrap();
        buf_write.clear();
    }
}
```

解密出来的flag是个二维码，扫码即可得flag

## magical_syscall

### 写在前面

- 这篇wp主要从出题人的角度来写，选手角度可以看看[Gift1a](https://github.com/XDSEC/miniLCTF_2023/blob/main/WriteUps/W4ntY0u/W4ntY0u.pdf) 的wp，写得非常详细。

- 题目源码以放在[github仓库](https://github.com/XDSEC/miniLCTF_2023/tree/main/Challenges/magical_syscall/src)，有兴趣可以去看看

### 几个要点

#### _init_array

_init_array是程序的初始化列表，其中注册的函数会在main函数开始之前被调用。

#### 两个简单的反调试

在初始化列表里注册了两个反调试函数。

一个是通过检查`/proc/self/status`文件中`TracePid`内容，判断是否被调试：

```c
nsigned __int64 sub_128C()
{
  char *i; // rax
  size_t v1; // rax
  FILE *stream; // [rsp+10h] [rbp-120h]
  char v4[264]; // [rsp+20h] [rbp-110h] BYREF
  unsigned __int64 v5; // [rsp+128h] [rbp-8h]

  v5 = __readfsqword(0x28u);
  stream = fopen("/proc/self/status", "r");
  for ( i = fgets(v4, 256, stream); i; i = fgets(v4, 256, stream) )
  {
    if ( strstr(v4, "TracerPid") )
    {
      v1 = strlen(v4);
      if ( atoi(&v4[v1 - 3]) )
      {
        puts("debugger detected, exit...");
        exit(1);
      }
    }
  }
  return v5 - __readfsqword(0x28u);
}
```

另一个是自定义了signal handler，并在程序运行10s后alarm，即当程序运行超过10s后会直接退出。

```c
unsigned int sub_1253()
{
  signal(14, handler);
  signal(5, (__sighandler_t)sub_1236);
  return alarm(0xAu);
}
```

由于这不是这道题的重点，所以没有在这为难大家，只是让大家了解一下。至于绕过只要给exit扬了就行。alarm信号在IDA调试时也可以直接忽略。

#### syscall

syscall即系统调用。我们一般的程序都运行在操作系统的用户空间，当需要进行一些更高权限的操作时，就需要通过系统调用进入内核执行代码，从而提高系统安全性和稳定性。

syscall函数是C标准库提供的一个构造syscall的工具函数，第一个参数为系统调用号，后面不定个需要的参数。参数按x64传参顺序依次赋给rdi、rsi、rdx、rcx

#### fork

fork是linux的一个系统调用，用来根据当前进程创建子进程。

fork函数是C标准库对fork syscall的封装。值得关注的是函数的返回值。返回值小于0说明创建子进程失败，在子进程中，返回值为0，而在父进程中，返回值为子进程的进程号(pid)。在实际编程中常用if分支通过返回值来区分父子进程，执行不同的代码。

#### ptrace

ptrace是linux的一个系统调用，一个进程可以通过ptrace查看甚至控制另一个进程的内部状态。大名鼎鼎的调试器gdb就是基于ptrace实现的，这里推荐一篇[文章](https://xz.aliyun.com/t/6882)

ptrace函数是C标准库对ptrace syscall的封装，函数原型如下：

```c
#include <sys/ptrace.h>       
long ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);
```

- request：要进行的ptrace操作
- pid：要操作的进程号
- addr：要监控/修改的内存地址
- data：要读取/写入的数据变量

常见的request操作可以看[这](https://man7.org/linux/man-pages/man2/ptrace.2.html)

#### user_regs_struct

这是一个定义了一系列寄存器的结构体，本身是专门为gdb写的，想看详细内容的话可以在C源文件里写一句`#include <sys/user.h>`，然后戳进去看就行了。

这个东西在进行ptrace操作（读写内存、寄存器）时很有用，可以在IDA里导入这个结构体方便分析。

#### 通过ptrace自定义syscall

syscall的过程是代码从用户态进入内核态的过程。如果我们把用户态换成子进程，内核态换成父进程，通过ptrace进行父进程对子进程内存空间的读写，模拟内核执行代码时的数据处理，就能实现自定义syscall。这是理解这道题运行逻辑的核心。

### 程序运行逻辑

下面按顺序梳理一下整个程序的运行逻辑

首先fork出子进程，子进程执行tracee函数，父进程执行tracer函数

```c
int sub_1D8B()
{
  signed int v1; // [rsp+Ch] [rbp-4h]

  v1 = fork();
  if ( v1 < 0 )
  {
    puts("failed to creat subprocess");
    exit(1);
  }
  if ( v1 )
    tracer(v1);
  return tracee();
}
```

子进程进入tracee函数，先ptrace TRACEME告诉操作系统自己要被父进程追踪，然后发送SIGCONT信号

```c
int sub_1386()
{
  ptrace(PTRACE_TRACEME, 0LL, 0LL, 0LL);
  return raise(SIGCONT);
}
```

父进程进入tracer函数，先通过waitpid函数等待子进程SIGCONT信号

```c
  waitpid(a1, &stat_loc, 0);
  if ( (unsigned __int8)stat_loc != 127 ) // !WIFSTOPPED(status) 如果子进程不是正常退出的，则进入if分支
  {
    puts("debugger detected, exit...");
    exit(1);
  }
  ptrace(PTRACE_SETOPTIONS, a1, 0LL, PTRACE_O_EXITKILL); // 如果子进程处于退出状态，则kill掉父进程
```

下面就是父进程中一个非常大的while循环，里面定义了一系列syscall，if判断的条件则是上面介绍的user_regs_struct里的`orig_rax`，即自定义syscall的系统调用号。

还有一个要点就是在while循环的开头和结尾都有这一句：

```c
ptrace(PTRACE_SYSCALL, a1, 0LL, 0LL);
```

用处是使子进程在每次syscall开始和结束时停下，把控制权交给父进程进行相应操作。

到这有一点要明确，父进程会一直在while循环里呆着，永远也不会执行到main函数，而只有子进程真正去执行main函数代码。父进程最终会走到以下两个分支之一得以退出：

```c
    if ( arg1 == 8888 )                         // FAIL
      break; 
    if ( arg1 == 9999 )                         // SUCCESS
    {
      puts("congratulations");
      exit(0);
    }
```

即程序最后check成功或失败的判断。

子进程执行的main函数非常简洁：

```c
void __fastcall __noreturn main(int a1, char **a2, char **a3)
{
  puts("input your flag:");
  while ( 1 )
    syscall(
      (unsigned int)dword_40AC[pc + 468],
      (unsigned int)dword_40AC[pc + 1 + 468],
      (unsigned int)dword_40AC[pc + 2 + 468],
      (unsigned int)dword_40AC[pc + 3 + 468]);
}
```

打印提示信息后，只有一个死循环，里面不断去执行syscall，正是这里执行的syscall会被父进程拦截并进行相应操作，syscall的参数即分别为系统调用号和所需参数。有些自定义的syscall并不需要3个参数，但由于这里并不会修改这些值，所以传几个多余的参数不会有任何影响。

### VM逆向

有经验的逆向壬应该一眼vm了，以上内容不关心靠猜也能七七八八。

这道题中vm的突破点应该在变量的识别。首先pc应该很容易看出来，毕竟每个syscall之后都会把它加上2或3或4，即那条指令的长度。其次是导入user_regs_struct之后就可以比较清晰的看懂几个参数了。

然后通过几个syscall对比来看应该也能看出来一些特殊的指令。

比如完全对称的push和pop：

```c
if ( arg1 == 3904 )                         // PUSH
    {
      dword_40B4 = ptrace(PTRACE_PEEKDATA, a1, &dword_40B4, 0LL);
      dword_40AC[++dword_40A4 + 4] = dword_40B4;
      ++pc;
      ptrace(PTRACE_POKEDATA, a1, &pc, (unsigned int)pc);
    }
    if ( arg1 == 3905 )                         // POP
    {
      dword_40B4 = dword_40AC[dword_40A4-- + 4];
      ++pc;
      ptrace(PTRACE_POKEDATA, a1, &pc, (unsigned int)pc);
    }
```

互相有联系的CMP、JE、JNE：

```c
if ( arg1 == 3906 )                         // CMP
    {
      if ( arg2 )
      {
        if ( arg2 == 1 )
          flag_zf = dword_40A8 == dword_40B4;
      }
      else
      {
        flag_zf = dword_40B0 == arg3;
      }
      pc += 3;
      ptrace(PTRACE_POKEDATA, a1, &pc, (unsigned int)pc);
    }
    if ( arg1 == 3907 )                         // JE
    {
      if ( flag_zf )
      {
        pc = arg2;
        ptrace(PTRACE_POKEDATA, a1, &pc, (unsigned int)arg2);
      }
      else
      {
        pc += 2;
        ptrace(PTRACE_POKEDATA, a1, &pc, (unsigned int)pc);
      }
    }
    if ( arg1 == 3908 )                         // JNE
    {
      if ( flag_zf )
      {
        pc += 2;
        ptrace(PTRACE_POKEDATA, a1, &pc, (unsigned int)pc);
      }
      else
      {
        pc = arg2;
        ptrace(PTRACE_POKEDATA, a1, &pc, (unsigned int)arg2);
      }
    }
```

然后基本就能看出来像`dword_40B0`这样的是一些通用寄存器(ax、bx、cx...)

再往下就是一些计算类的指令，比如INC、MOD、XOR、RESET（置零）应该也不难看出

难度较大的应该是ADD、MOV指令，因为这俩实际做的事取决于参数，会对不同的寄存器/内存地址进行ADD、MOV操作，这里就需要认真对参数进行分析，搞清楚具体的指令的含义。

整个VM实际实现了一个RC4加密，然后和密文比较的过程，出题时写的伪汇编看[这里](https://github.com/XDSEC/miniLCTF_2023/blob/main/Challenges/magical_syscall/src/assembly.txt)

另一个点在于XCHG指令，即实现两个值的交换，但这里因为并没有使用临时变量存储其中一个变量原先的值，所以是个假的交换，相当于`a = b; b = a;`这样的操作，这也是RC4的魔改点。

```c
    if ( arg1 == 3912 )                         // XCHG
    {
      *((_DWORD *)&mem + (unsigned int)dword_40B0) = *((_DWORD *)&mem + (unsigned int)dword_40AC[0]);
      *((_DWORD *)&mem + (unsigned int)dword_40AC[0]) = *((_DWORD *)&mem + (unsigned int)dword_40B0);
      ++pc;
      ptrace(PTRACE_POKEDATA, a1, &pc, (unsigned int)pc);
    }
```

### 最终脚本

```python
def rc4_init(s, key, key_len):
    j = 0
    for i in range(256):
        j = (j + s[i] + key[i%key_len])%256
        # tmp = s[i]
        s[i] = s[j]
        s[j] = s[i]

def rc4_generate_keystream(s, length):
    i = 0
    j = 0
    key_stream = []
    while length:
        i = (i + 1) % 256    # 可以保证每256次循环后s盒中的每个元素至少被交换一次
        j = (j + s[i]) % 256
        # tmp = s[i]
        s[i] = s[j]
        s[j] = s[i]
        key_stream.append(s[(s[i] + s[j]) % 256])
        length -= 1
    return key_stream

def main():
    key = [ord(i) for i in "MiniLCTF2023"]        # 准备一些变量
    key_len = len(key)
    # enc = [ord(i) for i in "llac$ys_laci9am_ht1w_en1hc@m_l@utr1v_a"]
    enc = [147, 163, 203, 201, 214, 211, 240, 213, 177, 26, 84, 155, 80, 203, 176, 178, 235, 15, 178, 141, 47, 230, 21, 203, 181, 61, 215, 156, 197, 129, 63, 145, 144, 241, 155, 171, 47, 242]
    enc_len = len(enc)
    cipher = [0] * enc_len

    s = [i for i in range(256)]    # 初始化s盒
    rc4_init(s, key, key_len)      # 使用key打乱s盒
    key_stream = rc4_generate_keystream(s[:], enc_len) # 生成密钥流
    # print(key_stream)
    for i in range(enc_len):     # 逐字节异或加密
        cipher[i] = enc[i] ^ key_stream[i]
    cipher = cipher[::-1]
    print("".join(chr(i) for i in cipher))
    # print(cipher)
    # print(len(cipher))

if __name__ == '__main__':
    main()
```

flag： a_v1rtu@l_m@ch1ne_w1th_ma9ical_sy$call
