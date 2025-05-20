#include <pthread.h>

void *worker(void *arg)
{
    printf("Hello World !\n");
    return(NULL);  /* Thread exits (dies) */
}

int main(int argc, char *argv[])
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, worker, NULL))
    {
        printf("Cannot create thread\n");
        exit(1);
    }

    printf("Main waits for thread to finish....\n");

    pthread_join(tid, NULL);

    exit(0);
}
