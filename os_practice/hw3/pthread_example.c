#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// 线程函数
void* thread_function(void* arg) {
    int thread_id = *((int*)arg);
    printf("Hello from thread %d!\n", thread_id);
    free(arg);  // 释放传递给线程的参数
    return NULL;
}

int main() {
    pthread_t threads[2];  // 存储线程 ID
    int* thread_args[2];   // 存储传递给线程的参数

    // 创建两个线程
    for (int i = 0; i < 2; i++) {
        thread_args[i] = (int*)malloc(sizeof(int));
        *thread_args[i] = i;
        pthread_create(&threads[i], NULL, thread_function, (void*)thread_args[i]);
    }

    // 等待线程结束
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
        free(thread_args[i]);  // 释放分配的内存
    }

    printf("Main thread completed.\n");
    return 0;
}
