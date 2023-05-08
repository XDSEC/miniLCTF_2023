#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <stdint.h>

#define SYS_DX_EXIT 9999
#define SYS_DX_FAIL_EXIT 8888
#define SYS_DX_READ 3900
#define SYS_DX_ADD 3901
#define SYS_DX_MOD 3902
#define SYS_DX_GETCHAR 3903
#define SYS_DX_PUSH 3904
#define SYS_DX_POP 3905
#define SYS_DX_CMP 3906
#define SYS_DX_JE 3907
#define SYS_DX_JNE 3908
#define SYS_DX_MOV 3909
#define SYS_DX_XOR 3910
#define SYS_DX_INC 3911
#define SYS_DX_XCHG 3912
#define SYS_DX_RESET 3913


struct VM {
    uint32_t ip;
    uint32_t sp;
    uint32_t ax;
    uint32_t bx;
    uint32_t cx;
    uint32_t dx;
    uint32_t ZF;
    uint32_t stack[64];    // 0-36 input
    uint32_t data[400];  // 0-255  s 256-267 key 268-305 enc
    uint32_t code[200]; 
};

struct VM vm = {
    .ip = 0,
    .sp = -1,
    .ax = 0,
    .bx = 0,
    .cx = 0,
    .dx = 0,
    .stack = {0},
    .data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 77, 105, 110, 105, 76, 67, 84, 70, 50, 48, 50, 51, 147, 163, 203, 201, 214, 211, 240, 213, 177, 26, 84, 155, 80, 203, 176, 178, 235, 15, 178, 141, 47, 230, 21, 203, 181, 61, 215, 156, 197, 129, 63, 145, 144, 241, 155, 171, 47, 242},
    .code = {3913, 0, 3903, 3904, 3911, 0, 3906, 0, 38, 3908, 2, 3913, 0, 3909, 1, 3911, 0, 3906, 0, 256, 3908, 13, 3913, 0, 3913, 1, 3901, 1, 0, 2, 3909, 0, 3902, 0, 12, 3901, 1, 1, 0, 3902, 1, 256, 3912, 3911, 0, 3906, 0, 256, 3908, 26, 3913, 0, 3913, 1, 3911, 0, 3901, 1, 0, 2, 3902, 1, 256, 3912, 3909, 2, 3901, 0, 0, 1, 3902, 0, 256, 3909, 4, 3905, 3910, 3909, 3, 3906, 1, 0, 3908, 90, 3906, 0, 38, 3908, 54, 9999, 8888, 0, 0, 0}
};

void alarmHandler() {
    puts("Never Give Up !!!!");
    exit(1);
}

void trapHandler() {
    puts("debugger detected, exit...");
    exit(1);
}

void setupSignal() __attribute__((constructor));
void setupSignal() {
    signal(SIGALRM, alarmHandler);
    signal(SIGTRAP, trapHandler);
    alarm(10);
}

void status_check() __attribute__((constructor));
void status_check() {
    FILE *fp = fopen("/proc/self/status","r");
    // printf("%p\n",fp);
    char buf[0x100];
    char *ptr = buf;
    while(fgets(ptr, 0x100, fp)) {
        if (strstr(ptr,"TracerPid")) {   
            int tracepid = 0;
            tracepid = atoi((char *)ptr + strlen(ptr) - 3);
            if (tracepid != 0) {
                puts("debugger detected, exit...");
                exit(1);
            }
        }
    }
}

void tracee() {
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    raise(SIGCONT);
}

void tracer(int child_pid) {
    int status;
    struct user_regs_struct regs;
    waitpid(child_pid, &status, 0);

    if (!WIFSTOPPED(status)) {
        puts("debugger detected, exit...");
        exit(1);
    }
    ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_EXITKILL);

    while(1) {
        ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
        waitpid(child_pid, &status, 0);
        ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
        if (regs.orig_rax == SYS_DX_FAIL_EXIT) {
            puts("try again");
            exit(1);
        }
        if (regs.orig_rax == SYS_DX_EXIT) {
            puts("congratulations");
            exit(0);
        }

        if (regs.orig_rax == SYS_DX_GETCHAR) {  // getchar dx
            regs.orig_rax = SYS_read;
            regs.rdi = 0;
            regs.rsi = (uint32_t *)&vm.dx;
            regs.rdx = 1;
            // printf("in getchar:\nrdi: %x rsi: %x rdx: %x\n", regs.rdi, regs.rsi, regs.rdx);
            ptrace(PTRACE_SETREGS, child_pid, 0, &regs);
            vm.ip += 1;
            ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
            
        }

        if (regs.orig_rax == SYS_DX_ADD) {
            uint32_t *reg = NULL;
            uint32_t *mem = NULL;
            uint32_t *offset = NULL;
            if (regs.rdi == 0) {
                reg = &vm.ax;
            } else if (regs.rdi == 1) {
                reg = &vm.bx;
            }
            
            if (regs.rsi == 0) {
                mem = vm.data;
            } else if (regs.rsi == 1) {
                mem = vm.data + 256;
            } else if (regs.rsi == 2) {
                mem = vm.data + 268;
            }

            if (regs.rdx == 0) {
                offset = &vm.ax;
            } else if (regs.rdx == 1) {
                offset = &vm.bx;
            } else if (regs.rdx == 2) {
                offset = &vm.cx;
            }

            *reg = *reg + *(mem + *offset);
            // ptrace(PTRACE_POKEDATA, child_pid, regs.rdi, *(uint32_t *)regs.rdi);
            vm.ip += 4;
            ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
        }
        
        if (regs.orig_rax == SYS_DX_MOD) {
            uint32_t *reg = NULL;
            if (regs.rdi == 0) {
                reg = &vm.ax;
            } else if (regs.rdi == 1) {
                reg = &vm.bx;
            }
            *reg = *reg % regs.rsi;
            // ptrace(PTRACE_POKEDATA, child_pid, regs.rdi, *(uint32_t *)regs.rdi);
            vm.ip += 3;
            ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
        }

        if (regs.orig_rax == SYS_DX_PUSH) { // push dx
            vm.dx = ptrace(PTRACE_PEEKDATA, child_pid, &vm.dx, 0);
            vm.sp ++;
            vm.stack[vm.sp] = vm.dx;
            // ptrace(PTRACE_POKEDATA, child_pid, &vm.stack[vm.sp], regs.rdi);
            // ptrace(PTRACE_POKEDATA, child_pid, &vm.sp, vm.sp);
            vm.ip += 1;
            // printf("in push\nvm.dx: %x, vm.sp: %x, vm.stack[vm.sp]: %x\n", vm.dx, vm.sp, vm.stack[vm.sp]);
            ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
        }

        if (regs.orig_rax == SYS_DX_POP) {
            // ptrace(PTRACE_POKEDATA, child_pid, &vm.dx, vm.stack[vm.sp]);
            vm.dx = vm.stack[vm.sp];
            vm.sp --;
            // ptrace(PTRACE_POKEDATA, child_pid, &vm.sp, vm.sp);
            vm.ip += 1;
            // puts("in pop");
            // printf("vm.dx: %x\n", vm.dx);
            ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
        }

        if (regs.orig_rax == SYS_DX_CMP) {
            if (regs.rdi == 0) {    // cmp cx, inum
                vm.ZF = (vm.cx == regs.rsi);
                // printf("in cmp 0\nvm.cx: %x regs.rsi: %x\n", vm.cx, regs.rsi);
                // ptrace(PTRACE_POKEDATA, child_pid, &vm.ZF, vm.ZF);
            } else if (regs.rdi == 1) {        // cmp ax, dx
                vm.ZF = (vm.ax == vm.dx);
            }
            vm.ip += 3;
            ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
        }

        if (regs.orig_rax == SYS_DX_JE) {
            if (vm.ZF) {
                vm.ip = regs.rdi;
                ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
            } else {
                vm.ip += 2;
                ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
            }
        }

        if (regs.orig_rax == SYS_DX_JNE) {
            if (!vm.ZF) {
                vm.ip = regs.rdi;
                // printf("jmp to %x\n", vm.ip);
                ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
            } else {
                vm.ip += 2;
                // printf("jne failed %x\n", vm.ip);
                ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
            }
        }

        if (regs.orig_rax == SYS_DX_MOV) {
            // puts("in mov");
            if (regs.rdi == 0) {    // mov ax, cx
                vm.ax = vm.cx;
                // printf("mov ax, cx  ax: %x\n", vm.ax);
            } else if (regs.rdi == 1) { // mov s[cx], cx
                *(vm.data + vm.cx) = vm.cx;
                // printf("mov s[cx], cx  cx: %x s[cx] = %x\n", vm.cx, *(vm.data + vm.cx));
            } else if (regs.rdi == 2) { // mov ax, s[cx]
                vm.ax = *(vm.data + vm.cx);
                // printf("mov ax, s[cx]  cx: %x ax: %x\n", vm.cx, vm.ax);
            } else if (regs.rdi == 3) { // mov ax, enc[cx-1]
                vm.ax = *(vm.data + 268 + vm.cx - 1);
                // printf("mov ax enc[cx-1]  ax: %x\n", vm.ax);
            } else if (regs.rdi == 4) { // mov ax, s[ax]
                vm.ax = *(vm.data + vm.ax);
            }
            vm.ip += 2;
            ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
        }

        if (regs.orig_rax == SYS_DX_XOR) {
            vm.dx = vm.dx ^ vm.ax;
            vm.ip += 1;
            // puts("in xor");
            // printf("vm.dx: %x vm.ax: %x\n", vm.dx, vm.ax);
            ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
        }

        if (regs.orig_rax == SYS_DX_INC) {
            uint32_t *reg;
            if (regs.rdi == 0) {    // inc cx
                reg = &vm.cx;
                // printf("in inc cx\nvm.cx: %x vm.cx_addr: %x\n", vm.cx, &vm.cx);
            } else if (regs.rdi == 1) { // inc bx
                reg = &vm.bx;
            }
            *reg = *reg + 1;
            // printf("reg_addr: %x, reg: %x\n", reg, *reg);
            vm.ip += 2;
            ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
        }

        if (regs.orig_rax == SYS_DX_XCHG) {
            // uint32_t tmp = *(vm.data + vm.bx);
            *(vm.data + vm.cx) = *(vm.data + vm.bx);
            *(vm.data + vm.bx) = *(vm.data + vm.cx);
            vm.ip += 1;
            ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
        }

        if (regs.orig_rax == SYS_DX_RESET) {
            uint32_t *reg = NULL;
            if (regs.rdi == 0) {    // reset cx, 0
                // printf("in reset cx\n");
                reg = &vm.cx;
            } else if (regs.rdi == 1) { // reset bx, 0
                // printf("in reset bx\n");
                reg = &vm.bx;
            }
            *reg = 0;
            // printf("reg_addr: %x reg_val: %x\n");
            vm.ip += 2;
            ptrace(PTRACE_POKEDATA, child_pid, &vm.ip, vm.ip);
        }

        ptrace(PTRACE_SYSCALL, child_pid, 0, 0);
        waitpid(child_pid, &status, 0);
    }
}

int ptrace_check() __attribute__((constructor));
int ptrace_check() {
    int pid = fork();
    if (pid < 0) {
        puts("failed to creat subprocess");
        exit(1);
    } else if (pid == 0) {
        tracee();
    } else {
        tracer(pid);
    }
}



int main() {
    puts("input your flag:");
    while(1) {
        uint32_t ip = vm.ip;
        syscall(vm.code[ip], vm.code[ip+1], vm.code[ip+2], vm.code[ip+3]);
        // printf("ip : %x\n", ip);
        // printf("main vm.dx: %x\n", vm.dx);
    }
}

