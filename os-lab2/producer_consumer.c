#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 10
#define PRODUCT_NUM 15

int put_idx = 0;
int get_idx = 0;
char buffer[BUFFER_SIZE][10];

sem_t empty_slots;
sem_t filled_slots;
sem_t mutex_sem;
char box[10][10] = {
    "Apple", "Banana", "Orange", "Grape", "Lemon",
    "Peach", "Cherry", "Berry", "Plum", "Mango"
};

void* producer(void* arg) {
    for (int i = 0; i < PRODUCT_NUM; i++) {
        sem_wait(&empty_slots);
        sem_wait(&mutex_sem);
        strcpy(buffer[put_idx], box[rand()%10]);
        printf("生产了%s,序号为%d\n", buffer[put_idx], put_idx);
        put_idx = (put_idx + 1) % BUFFER_SIZE;
        sem_post(&mutex_sem);
        sem_post(&filled_slots);
    }
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    for (int i = 0; i < PRODUCT_NUM; i++) {
        sem_wait(&filled_slots);
        sem_wait(&mutex_sem);
        printf("消费了%s,序号为%d\n", buffer[get_idx], get_idx);
        get_idx = (get_idx + 1) % BUFFER_SIZE;
        sem_post(&mutex_sem);
        sem_post(&empty_slots);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t producer_thread, consumer_thread;

    if (sem_init(&empty_slots, 0, BUFFER_SIZE) != 0) {
        perror("sem_init empty_slots failed");
        return 1;
    }
    if (sem_init(&filled_slots, 0, 0) != 0) {
        perror("sem_init filled_slots failed");
        sem_destroy(&empty_slots);
        return 1;
    }
    if (sem_init(&mutex_sem, 0, 1) != 0) {
        perror("sem_init mutex_sem failed");
        sem_destroy(&empty_slots);
        sem_destroy(&filled_slots);
        return 1;
    }

    if (pthread_create(&producer_thread, NULL, producer, NULL) != 0) {
        perror("Producer thread creation failed");
        sem_destroy(&empty_slots);
        sem_destroy(&filled_slots);
        sem_destroy(&mutex_sem);
        return 1;
    }
    if (pthread_create(&consumer_thread, NULL, consumer, NULL) != 0) {
        perror("Consumer thread creation failed");
        sem_destroy(&empty_slots);
        sem_destroy(&filled_slots);
        sem_destroy(&mutex_sem);
        return 1;
    }

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty_slots);
    sem_destroy(&filled_slots);
    sem_destroy(&mutex_sem);

    printf("结束了");
    return 0;
}
