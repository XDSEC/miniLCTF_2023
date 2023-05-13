// gcc pwn.c -o pwn -no-pie -fno-pic -fno-stack-protector
#include <stdio.h>

void pwn() {
    char buf[0x10];
    puts("Welcome to 2023 miniL! Let's play a very eazy game! What's your name?");
    gets(buf);
    puts("Good luck!");
}

int main() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    pwn();
    return 0;
}
