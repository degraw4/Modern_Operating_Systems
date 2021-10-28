#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <pthread.h>
#define MAX 100

// 条件变量和信号量最大的区别：条件变量可以一次性唤醒多个

pthread_mutex_t mutex;
pthread_cond_t proc, cons;
int buffer = 0;

void* producer(void* ptr){
    for(int i = 1; i <= MAX; ++i){
        pthread_mutex_lock(&mutex);
        while(buffer != 0)
            pthread_cond_wait(&proc, &mutex);
        buffer = i;
        printf("Produce %d\n", i);
        // 顺序任意
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cons);
    }
    pthread_exit(0);
}

void* consumer(void* ptr){
    for(int i = 1; i <= MAX; ++i){
        pthread_mutex_lock(&mutex);
        while(buffer == 0)
            pthread_cond_wait(&cons, &mutex);
        buffer = 0;
        printf("Consume %d\n", i);
        pthread_cond_signal(&proc);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}


int main(int argc, char** argv)
{
    pthread_t proc_t, cons_t;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&proc, NULL);
    pthread_cond_init(&cons, NULL);
    pthread_create(&proc_t, 0, producer, 0);
    pthread_create(&cons_t, 0, consumer, 0);
    pthread_join(proc_t, 0);
    pthread_join(cons_t, 0);
    pthread_cond_destroy(&proc);
    pthread_cond_destroy(&cons);
    pthread_mutex_destroy(&mutex);

    return 0;
}
