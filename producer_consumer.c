#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
 
#define BUFF_SIZE 5 /* size of the buffer */
#define NP 3 /* total number of producers */
#define NC 3 /* total number of consumers */
#define NITERS 4 /* number of items produced/consumed */
 
typedef struct {
    int buf[BUFF_SIZE]; /* shared var */
    int in; /* buf[in%BUFF_SIZE] is the first empty slot */
    int out; /* buf[out%BUFF_SIZE] is the first full slot */
    sem_t full; /* keep track of the number of full spots */
    sem_t empty; /* keep track of the number of empty spots */
    pthread_mutex_t mutex; /* enforce mutual exclusion to shared
data */
} sbuf_t;
sbuf_t shared;
 
void *Producer(void *arg) {
    int i, item, index;
    index = (int) arg;
 
    for (i = 0; i < NITERS; i++) {
/* Produce item */
        item = i;
/* Prepare to write item to buf */
/* If there are no empty slots, wait */
        sem_wait(&shared.empty);
/* If another thread uses the buffer, wait */
        pthread_mutex_lock(&shared.mutex);
        shared.buf[shared.in] = item;
        shared.in = (shared.in + 1) % BUFF_SIZE;
        printf("[P%d] Producing %d ...\n", index, item);
        fflush(stdout);
/* Release the buffer */
        pthread_mutex_unlock(&shared.mutex);
/* Increment the number of full slots */
        sem_post(&shared.full);
/* Interleave producer and consumer execution */
        if (i % 2 == 1) sleep(1);
    }
 
    return NULL;
}
 
void *Consumer(void *arg) {
    int i, item, index;
    index = (int) arg;
    for (i = NITERS; i > 0; i--) {
        sem_wait(&shared.full);
        pthread_mutex_lock(&shared.mutex);
        item = i;
        item = shared.buf[shared.out];
        shared.out = (shared.out + 1) % BUFF_SIZE;
        printf("[C%d] Consuming %d ...\n", index, item);
        fflush(stdout);
/* Release the buffer */
        pthread_mutex_unlock(&shared.mutex);
/* Increment the number of full slots */
        sem_post(&shared.empty);
/* Interleave producer and consumer execution */
        if (i % 2 == 1) sleep(1);
    }
    return NULL;
}
 
int main() {
    pthread_t idP, idC;
    int index;
    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFF_SIZE);
    pthread_mutex_init(&shared.mutex, NULL);
    for (index = 0; index < NP; index++) {
    /* Create a new producer */
        pthread_create(&idP, NULL, Producer, (void *) index);
    }
    /* create a new Consumer */
    for (index = 0; index < NC; index++) {
        pthread_create(&idC, NULL, Consumer, (void *) index);
    }
    pthread_exit(NULL);
}
