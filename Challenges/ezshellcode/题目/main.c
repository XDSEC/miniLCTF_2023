#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <seccomp.h>
#include <unistd.h>
#include <sys/mman.h>

char ooooo0ooo0o[0x100];
void O0ooo0oo0o()
{
    close(0);
    close(1);
    close(2);
    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_KILL);
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, 60, 0); // exit
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, 41, 0); // socket
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, 42, 0); // connect
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, 40, 0); // sendfile
    seccomp_load(ctx);
}
int o0o0o000o()
{
    puts("I'll make sure flag is ready.");
    int fd1, fdl;
    char bufl[1024], buf1[1024];
    ssize_t nread1, nreadl;

    // 打开flag1文件
    fd1 = open("flag1", O_RDONLY);
    if (fd1 < 0) {
        puts("flag1 file doesn't exit");
        return 0;
    }
    nread1 = read(fd1, bufl, sizeof(bufl));
    if (nread1 <= 0) {
        puts("flag1 file is empty");
        return 0;
    }

    // 打开flag2文件
    fdl = open("flag2", O_RDONLY);
    if (fdl < 0) {
        puts("flag2 file doesn't exit");
        return 0;
    }
    nreadl = read(fdl, buf1, sizeof(buf1));
    if (nreadl <= 0) {
        puts("flag2 file is empty");
        return 0;
    }
    puts("Well, flag is ready.");
    close(fd1);
    close(fd1);
    memset(buf1, 0, sizeof(buf1));
    memset(buf1, 0, sizeof(buf1));
    O0ooo0oo0o();
    return 1;
}
void OooO0OOo()
{
    setvbuf(stdin, 0LL, 2, 0LL);
    setvbuf(stdout, 0LL, 2, 0LL);
    setvbuf(stderr, 0LL, 2, 0LL);
}
int main()
{
    OooO0OOo();
    puts("Welcome to MiniL2023!!! Enjoy~");
    puts("Well, Let's play a game to warm up!");
    char *oo0Oo = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC, 34, -1, 0);
    memset(oo0Oo, 0x90, 0x1000);
    unsigned int length = read(0, ooooo0ooo0o, 0x40);
    memcpy(oo0Oo, "H1\xc0H1\xdbH1\xc9H1\xd2H1\xffH1\xf6H1\xedH1\xe4M1\xc0M1\xc9M1\xd2M1\xdbM1\xe4M1\xedM1\xf6M1\xff", 48);
    memcpy(oo0Oo + 48, ooooo0ooo0o, length);
    if(o0o0o000o())
        ((void (*)(void))oo0Oo)();
}