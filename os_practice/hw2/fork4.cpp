#include <stdio.h>
#include <unistd.h>

int main() {
    printf("循环次数 | 进程角色 | PPID  | PID   | fork返回值\n");
    printf("------------------------------------------------\n");

    for (int i = 0; i < 2; i++) {
        pid_t fpid = fork();
        
        if (fpid == 0) {
            printf("%-8d| 子进程  | %-5d| %-5d| ➔ %d\n", 
                  i, getppid(), getpid(), fpid);
        } else {
            printf("%-8d| 父进程  | %-5d| %-5d| ➔ %d\n", 
                  i, getppid(), getpid(), fpid);
        }
    }
    return 0;
}
