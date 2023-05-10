# maze_aot wp by dr3

输入%llx，输入十六进制的数储存，之后调用
maze_walk函数。

## maze_walk

函数很复杂，但是基本原理是走64步以内到达目标

目标函数可以通过查看字符串获得

基本思路就是acm基础图论。

先写idapython脚本提取图

之后只需要跑一个dfs或者bfs，然后得到最短路径就可以

使用networkx库也可以很快的得到目标最短路径

之后判断基本快是否是0，1，通过判断目标块和块中存在的是
jz还是jnz输出0，1

最后转回16进制，不要忘了倒转。

```python
import idaapi

function_address = 0x1500

function = idaapi.get_func(function_address)

graph = dict()
cfg = idaapi.FlowChart(function)
exclusive_nodes = list()
target_length = 65
first = None
end = 9177
jmp_dict = dict()
for block in cfg:
    graph[block.start_ea] = list()
    start_address = block.start_ea
    end_address = block.end_ea
    if first is None:
        print("Starting")
        first = start_address
    ea = 0
    if end_address - start_address <= 5:
        print("found jmp block")
        exclusive_nodes.append(start_address)
        for succ in block.succs():  # is a jmp block, ignore it
            graph[block.start_ea].append(succ.start_ea)
        continue
    flag = 0
    tgt = 0
    while (end_address - ea) != start_address:

        if idc.GetDisasm(end_address - ea).startswith("jnz"):
            # print(int(idc.GetDisasm(end_address - ea)[-4::],16))
            flag = 1
            tgt = int(idc.GetDisasm(end_address - ea)[-4::],16)
        elif idc.GetDisasm(end_address - ea).startswith("jz"):
            flag = 2
            tgt = int(idc.GetDisasm(end_address - ea)[-4::], 16)
        ea += 1
    if flag != 0:
        jmp_dict[block.start_ea] = (flag,tgt)
    for succ in block.succs():
        graph[block.start_ea].append(succ.start_ea)



print(jmp_dict)


def BFS(grap, star):                    # BFS算法
    queue = []                          # 定义一个队列
    seen = set()                        # 建立一个集合，集合就是用来判断该元素是不是已经出现过
    queue.append(star)                  # 将任一个节点放入
    seen.add(star)                      # 同上
    parent = {star:None}                #存放parent元素
    while (len(queue) > 0):             # 当队列里还有东西时
        ver = queue.pop(0)              # 取出队头元素
        notes = grap[ver]               # 查看grep里面的key,对应的邻接点
        for i in notes:                 # 遍历邻接点
            if i not in seen:           # 如果该邻接点还没出现过
                queue.append(i)         # 存入queue
                seen.add(i)             # 存入集合
                parent[i] = ver         #将元素对应的parent元素存入字典中
    return parent


# path = bfs(graph, first, end)
parent = BFS(graph,first)

a = end
while a != None:
    # print(a)
    p.append(a)
    a = parent[a]
path = p

print(f"路径: {path}")
for i in range(len(path) - 1):
    if path[i] not in exclusive_nodes and path[i] != 5376:

        next = path[i + 1]
        if (jmp_dict[path[i]][1] == next and jmp_dict[path[i]][0] == 1):
            print('1', end="")
        elif (jmp_dict[path[i]][1] != next and jmp_dict[path[i]][0] == 1):
            print("0", end='')
        elif (jmp_dict[path[i]][1] == next and jmp_dict[path[i]][0] == 2):
            print("0", end='')
        elif (jmp_dict[path[i]][1] != next and jmp_dict[path[i]][0] == 2):
            print("1", end='')
        else:
            print("ERR")

```