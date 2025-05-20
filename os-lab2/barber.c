#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CHAIRS 6

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barber_sleep = PTHREAD_COND_INITIALIZER;
pthread_cond_t customer_wait = PTHREAD_COND_INITIALIZER;
int num_waiting_customers = 0;

void* barber(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (num_waiting_customers == 0) {
            printf("��ʦ��˯��\n");
            pthread_cond_wait(&barber_sleep, &mutex);
        }
        printf("��ʦ����\n");
        num_waiting_customers--;
        pthread_mutex_unlock(&mutex);
		usleep(rand() % 1000000); 
    }
}

void* customer(void* arg) {
    pthread_mutex_lock(&mutex);
    if (num_waiting_customers < NUM_CHAIRS) {
        num_waiting_customers++;
        printf("�˿������ˣ�Ŀǰ����%d���˿��ڵȴ�\n", num_waiting_customers);
        pthread_cond_signal(&barber_sleep);
        pthread_mutex_unlock(&mutex);
        usleep(rand() % 1000000); 
    } else {
        printf("û��\n");
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t barber_thread, customer_threads[NUM_CHAIRS + 6];
    pthread_create(&barber_thread, NULL, barber, NULL);
    for (int i = 0; i < NUM_CHAIRS + 6; i++) {
        pthread_create(&customer_threads[i], NULL, customer, NULL);
        usleep(rand() % 1000000); 
    }
    pthread_join(barber_thread, NULL);
    for (int i = 0; i < NUM_CHAIRS + 6; i++) {
        pthread_join(customer_threads[i], NULL);
    }
    return 0;
}