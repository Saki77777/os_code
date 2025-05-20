#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("fork failed");
        return 1;
    } else if (child_pid == 0) {
        printf("Child process: My PID is %d\n", getpid());
        _exit(42);
    } else {
        printf("Parent process: Child process ID is %d\n", child_pid);
        
        int status;
        waitpid(child_pid, &status, 0);        
        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child terminated abnormally\n");
        }
    }
    return 0;
}
