// gcc pwn.c -o pwn
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

union {
    long long num[3];
    void (*funcs[3])(void);
    char readf[3][8];
} F;

long long libc;

char *cmd = "readelf ./libc.so.6 -s | tail -n 3006 | grep FUNC | awk '{print $2}'";

void check() {
    char buf[0x20], s[3][0x20];
    int flag[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++)
        sprintf(s[i], "%016llx", (long long) F.funcs[i] - libc);
    FILE *f = popen(cmd, "r");
    if ((long long)f > 0) {
        while (fscanf(f, "%s", buf) != EOF) {
            for (int i = 0; i < 3; i++)
                flag[i] |= strcmp(s[i], buf) == 0;
        }
        pclose(f);
        f = NULL;
    }
    else {
        puts("popen failed!");
        exit(-1);
    }
    if (flag[0] & flag[1] & flag[2])
        return;
    puts("Not libc symbols!");
    exit(-1);
}

int main() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    libc = (long long)&printf - 0x00060770;
    printf("gift: %p\n", (void *)libc);
    char buf[0x10];
    for (int i = 0; i < 3; i++)
        read(0, &F.readf[i], 0x8);
    check();
    puts("good job!");
    for (int i = 0; i < 3; i++)
        F.funcs[i]();
    return 0;
}
