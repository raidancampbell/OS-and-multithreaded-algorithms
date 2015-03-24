//============================================================================
//          THE CODE BELOW WAS HEAVILY RECYCLED FROM THIS SOURCE:
// Name        : posixExample.c
// Author      : Daniel M. Savel
// CaseID      : dxs221
// Description : POSIX Examples, POSIX Threads and POSIX Semaphores
//		 Simple Threaded Readers-Writers Simulation
// Compile CMD : gcc posixExample.c -lpthread
//============================================================================


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define DEPOSITOR_COUNT 10
#define WITHDRAWER_COUNT 5
#define RAND_SEED 2114

//Pointers to methods
void *depositorThread(void *threadId);
void *withdrawerThread(void *threadId);

void my_sleep(int limit);

//POSIX Semaphores and shared variables
sem_t wrt, mutex;
int readcount = 0;

struct threadInfo {int threadId; };

struct threadInfo depositorIDs[DEPOSITOR_COUNT];
struct threadInfo withdrawerIDs[WITHDRAWER_COUNT];

int main(void)
{
    pthread_t depositors[DEPOSITOR_COUNT];
    pthread_t withdrawers[WITHDRAWER_COUNT];
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
    for(i = 0; i < DEPOSITOR_COUNT; i++)
    {
        depositorIDs[i].threadId = i;
        int result = pthread_create(&depositors[i], &attr, depositorThread, (void *) &depositorIDs[i]);
        if(result == -1)
        {
            perror("Thread Creation: Depositor");
            exit(EXIT_FAILURE);
        }
    }

    //Spawn Writers
    for(i = 0; i < WITHDRAWER_COUNT; i++)
    {
        withdrawerIDs[i].threadId = i;
        int result = pthread_create(&withdrawers[i], &attr, withdrawerThread, (void *) &withdrawerIDs[i]);
        if(result == -1)
        {
            perror("Thread Creation: Withdrawer");
            exit(EXIT_FAILURE);
        }
    }

    //Wait for all the threads to finish
    for(i = 0; i < DEPOSITOR_COUNT; i++)
    {
        int result = pthread_join(depositors[i], &status);
        if(result == -1)
        {
            perror("Thread Join: Depositor");
            exit(EXIT_FAILURE);
        }
    }

    for(i = 0; i < WITHDRAWER_COUNT; i++)
    {
        int result = pthread_join(withdrawers[i], &status);
        if(result == -1)
        {
            perror("Thread Join: Withdrawer");
            exit(EXIT_FAILURE);
        }
    }
}

void *depositorThread(void *threadId)
{
    struct threadInfo * info;
    info = (struct threadInfo *) threadId;
    int id = info->threadId;

    my_sleep(100);	//Simulate being idle for 1-100ms

    //Depositor Entry
    sem_wait(&mutex);
    readcount++;

    if(readcount == 1) sem_wait(&wrt);
    sem_post(&mutex);

    //Reader CS
    printf("Depositor %d enters CS\n", id);
    my_sleep(10); //Simulates a read operation taking 1-10ms
    printf("Depositor %d exits CS\n", id);

    //Depositor Cleanup
    sem_wait(&mutex);
    readcount--;
    if(readcount == 0)
    {
        sem_post(&wrt);
    }
    sem_post(&mutex);
    pthread_exit(NULL);
}


void *withdrawerThread(void *threadId)
{
    struct threadInfo * info;
    info = (struct threadInfo *) threadId;
    int id = info->threadId;

    my_sleep(500); //Simulate being idle for 1-500ms

    //Withdrawer Entry
    sem_wait(&wrt);

    //Withdrawer CS
    printf("Withdrawer %d enters CS\n", id);
    my_sleep(50); //Simulate a write operation taking 1-50ms
    printf("Withdrawer %d exits CS\n", id);

    //Withdrawer Cleanup
    sem_post(&wrt);
    pthread_exit(NULL);
}


//Puts the calling thread to sleep to simulate both random start times and random workloads
void my_sleep(int limit)
{
    struct timespec time_ns;
    int duration = (int) random() % limit + 1;
    time_ns.tv_sec = 0;
    time_ns.tv_nsec = duration * 1000000;
    int result = nanosleep(&time_ns, NULL);
    if (result != 0) {
        perror("Nanosleep");
        exit(EXIT_FAILURE);
    }
}