//
// Created by Aidan Campbell on 3/23/15.
//
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include "main.h"

#define READER_COUNT 10
#define WRITER_COUNT 5
#define RAND_SEED 2114

//Pointers to methods
void *readerThread(void *threadId);
void *writerThread(void *threadId);

void my_sleep(int limit);

//POSIX Semaphores and shared variables
sem_t wrt, mutex;
int readcount = 0;

struct threadInfo
{
    int threadId;
};

struct threadInfo readerIDs[READER_COUNT];
struct threadInfo writerIDs[WRITER_COUNT];

int main(void)
{
    pthread_t readers[READER_COUNT];
    pthread_t writers[WRITER_COUNT];
    pthread_attr_t attr;
    void *status;

    //Seed the Random Number Generator
    srandom(RAND_SEED);

    //Creating a set of attributes to send to the threads
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //Initializing the WRT and MUTEX semaphores
    sem_init(&wrt, 0 , 1);
    sem_init(&mutex, 0, 1);

    //Spawn Readers
    int i;
    for(i = 0; i < READER_COUNT; i++)
    {
        readerIDs[i].threadId = i;
        int result = pthread_create(&readers[i], &attr, readerThread, (void*) &readerIDs[i]);
        if(result == -1)
        {
            perror("Thread Creation: Reader");
            exit(EXIT_FAILURE);
        }
    }

    //Spawn Writers
    for(i = 0; i < WRITER_COUNT; i++)
    {
        writerIDs[i].threadId = i;
        int result = pthread_create(&writers[i], &attr, writerThread, (void*) &writerIDs[i]);
        if(result == -1)
        {
            perror("Thread Creation: Writer");
            exit(EXIT_FAILURE);
        }
    }

    //Wait for all the threads to finish
    for(i = 0; i < READER_COUNT; i++)
    {
        int result = pthread_join(readers[i], &status);
        if(result == -1)
        {
            perror("Thread Join: Reader");
            exit(EXIT_FAILURE);
        }
    }

    for(i = 0; i < WRITER_COUNT; i++)
    {
        int result = pthread_join(writers[i], &status);
        if(result == -1)
        {
            perror("Thread Join: Writer");
            exit(EXIT_FAILURE);
        }
    }
}

void *readerThread(void *threadId)
{
    struct threadInfo * info;
    info = (struct threadInfo *) threadId;
    int id = info->threadId;

    my_sleep(100);	//Simulate being idle for 1-100ms

    //Reader Entry
    sem_wait(&mutex);
    readcount++;
    if(readcount == 1)
    {
        sem_wait(&wrt);
    }
    sem_post(&mutex);

    //Reader CS
    printf("Reader %d enters CS\n", id);
    my_sleep(10); //Simulates a read operation taking 1-10ms
    printf("Reader %d exits CS\n", id);

    //Reader Cleanup
    sem_wait(&mutex);
    readcount--;
    if(readcount == 0)
    {
        sem_post(&wrt);
    }
    sem_post(&mutex);
    pthread_exit(NULL);
}


void *writerThread(void *threadId)
{
    struct threadInfo * info;
    info = (struct threadInfo *) threadId;
    int id = info->threadId;

    my_sleep(500); //Simulate being idle for 1-500ms

    //Writer Entry
    sem_wait(&wrt);

    //Writer CS
    printf("Writer %d enters CS\n", id);
    my_sleep(50); //Simulate a write operation taking 1-50ms
    printf("Writer %d exits CS\n", id);

    //Writer Cleanup
    sem_post(&wrt);
    pthread_exit(NULL);
}


//Puts the calling thread to sleep to simulate both random start times and random workloads
void my_sleep(int limit)
{
    struct timespec time_ns;
    int duration = random() % limit + 1;
    time_ns.tv_sec = 0;
    time_ns.tv_nsec = duration * 1000000;
    int result = nanosleep(&time_ns, NULL);
    if (result != 0)
    {
        perror("Nanosleep");
        exit(EXIT_FAILURE);
    }
}

//returns either DEPOSIT or WITHDRAWAL, useful for random generations
int randomType(){
    if(rand() % 2 == 0) return DEPOSIT;
    else return WITHDRAWAL;
}

//returns a random value between 0 and 1000, useful for random generations
int randomAmount(){
    return rand() % 1000;
}