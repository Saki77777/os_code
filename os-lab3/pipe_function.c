#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int fx(int x) {
    if (x == 1)
        return 1;
    return x * fx(x - 1);
}

int fy(int y) {
    if (y == 1 || y == 2)
        return 1;
    return fy(y - 1) + fy(y - 2);
}

int main() {
    pid_t pid_x, pid_y;
    int x_val, y_val;
    int fx_result, fy_result;

    int pipe_fx[2];
    int pipe_fy[2];

    printf("Please input x and y:\n");
    scanf("%d %d", &x_val, &y_val);

    if (pipe(pipe_fx) == -1) {
        perror("pipe_fx creation failed");
        exit(EXIT_FAILURE);
    }

    pid_x = fork();
    if (pid_x < 0) {
        perror("fork_x failed");
        exit(EXIT_FAILURE);
    } else if (pid_x == 0) {
        close(pipe_fx[0]);
        int result_x = fx(x_val);
        printf("f(x) = %d (calculated by child %d)\n", result_x, getpid());
        write(pipe_fx[1], &result_x, sizeof(result_x));
        close(pipe_fx[1]);
        exit(0);
    }

    if (pipe(pipe_fy) == -1) {
        perror("pipe_fy creation failed");
        exit(EXIT_FAILURE);
    }

    pid_y = fork();
    if (pid_y < 0) {
        perror("fork_y failed");
        exit(EXIT_FAILURE);
    } else if (pid_y == 0) {
        close(pipe_fy[0]);
        int result_y = fy(y_val);
        printf("f(y) = %d (calculated by child %d)\n", result_y, getpid());
        write(pipe_fy[1], &result_y, sizeof(result_y));
        close(pipe_fy[1]);
        exit(0);
    }

    close(pipe_fx[1]);
    close(pipe_fy[1]);

    waitpid(pid_x, NULL, 0);
    waitpid(pid_y, NULL, 0);

    read(pipe_fx[0], &fx_result, sizeof(fx_result));
    read(pipe_fy[0], &fy_result, sizeof(fy_result));

    close(pipe_fx[0]);
    close(pipe_fy[0]);

    printf("f(x, y) = %d + %d = %d (calculated by parent %d)\n", fx_result, fy_result, fx_result + fy_result, getpid());

    return 0;
}
