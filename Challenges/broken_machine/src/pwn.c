// gcc pwn.c -o pwn -no-pie -fno-pic -z now
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>
#include <linux/seccomp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/prctl.h>

char buf[0x400];

void handler(int signo) {
    puts("Oh, my machine seems broken.");
    puts("Thank you for you test. I'll improve it!");
    exit(0);
}

static void sandbox() {
  static unsigned char filter[] = {32,0,0,0,4,0,0,0,21,0,0,20,62,0,0,192,32,0,0,0,0,0,0,0,53,0,18,0,0,0,0,64,21,0,17,0,59,0,0,0,21,0,16,0,66,1,0,0,21,0,15,0,2,0,0,0,21,0,14,0,1,1,0,0,21,0,13,0,0,0,0,0,21,0,12,0,19,0,0,0,21,0,11,0,17,0,0,0,21,0,10,0,39,1,0,0,21,0,9,0,57,0,0,0,21,0,8,0,58,0,0,0,21,0,7,0,41,0,0,0,21,0,6,0,42,0,0,0,21,0,5,0,43,0,0,0,21,0,4,0,157,0,0,0,21,0,3,0,101,0,0,0,21,0,2,0,9,0,0,0,21,0,1,0,10,0,0,0,6,0,0,0,0,0,255,127,6,0,0,0,0,0,0,0};
  struct prog {
    unsigned short len;
    unsigned char *filter;
  } rule = {
    .len = sizeof(filter) >> 3,
    .filter = filter
  };
  if(prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0) { perror("prctl(PR_SET_NO_NEW_PRIVS)"); exit(2); }
  if(prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &rule) < 0) { perror("prctl(PR_SET_SECCOMP)"); exit(2); }
}

void machine() {
    void (*fun)() = (void *)0x1000;
    void *addr = mmap(fun, 0x1000, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    puts("Prepare code...");
    sprintf(addr, buf);
    sleep(1);
    puts("Setup sandbox...");
    mprotect(addr, 0x1000, PROT_EXEC | PROT_READ);
    sandbox();
    sleep(1);
    puts("Finished!");
    fun();
}

int main() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    puts("Hi there! I've made a shellcode machine. Could you please input you code to test my machine? Thank you!");
    signal(SIGSEGV, handler);
    fgets(buf, 0x400, stdin);
    int cnt = 0;
    for (int i = 0; i < 0x400; i++)
      cnt += buf[i] == 'n';
    if (cnt > 1)
      return 0;
    machine();
    return 0;
}
