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
