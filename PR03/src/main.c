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
#include "LL.h"

#define DEPOSITOR_COUNT 500
#define WITHDRAWER_COUNT 500
#define RAND_SEED 2114

//Pointers to methods
void *depositorThread(void *threadId);
void *withdrawerThread(void *threadId);
int randomVal();
void my_sleep(int limit);

//POSIX Semaphores and shared variables
sem_t wlist, mutex;
int balance = 500;
int wcount = 0;
struct test_struct *list = NULL;


struct threadInfo {int threadId; };

struct threadInfo depositorIDs[DEPOSITOR_COUNT];
struct threadInfo withdrawerIDs[WITHDRAWER_COUNT];

int main(void) {
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
    sem_init(&wlist, 0, 0);
    sem_init(&mutex, 0, 1);

    //Spawn Readers
    int i;
    for(i = 0; i < DEPOSITOR_COUNT; i++) {
        depositorIDs[i].threadId = i;
        int result = pthread_create(&depositors[i], &attr, depositorThread, (void *) &depositorIDs[i]);
        if(result < 0) {
            perror("Thread Creation: Depositor");
            exit(EXIT_FAILURE);
        }
    }

    //Spawn Writers
    for(i = 0; i < WITHDRAWER_COUNT; i++) {
        withdrawerIDs[i].threadId = i;
        int result = pthread_create(&withdrawers[i], &attr, withdrawerThread, (void *) &withdrawerIDs[i]);
        if(result < 0) {
            perror("Thread Creation: Withdrawer");
            exit(EXIT_FAILURE);
        }
    }

    //Wait for all the threads to finish
    for(i = 0; i < DEPOSITOR_COUNT; i++) {
        int result = pthread_join(depositors[i], &status);
        if(result < 0) {
            perror("Thread Join: Depositor");
            exit(EXIT_FAILURE);
        }
    }

    for(i = 0; i < WITHDRAWER_COUNT; i++) {
        int result = pthread_join(withdrawers[i], &status);
        if(result < 0) {
            perror("Thread Join: Withdrawer");
            exit(EXIT_FAILURE);
        }
    }
	return EXIT_SUCCESS;
}

void *depositorThread(void *threadId)
{
    /*
Depositing Customer:// Assume that the local variable deposit (int) contains the amount to be deposited.
wait (mutex);
balance := balance + deposit;
if (wcount = 0) {
    signal (mutex)
} else if (FirstRequestAmount (LIST) > balance){
    signal (mutex)
} else {
    signal (wlist); // Deposit has taken place.
}
    */

    my_sleep(100);	//Simulate being idle for 1-100ms

    //Depositor Entry
    sem_wait(&mutex);
    int depositAmount = randomVal();
    balance += depositAmount;
    printf("\nBalance: %d\t\tPost deposit of: %d", balance, depositAmount);
    if(wcount==0) sem_post(&mutex);
    else if (firstRequestAmount(list) > balance) {
        printf("\nThat deposit allowed the waiting withdrawal of: %d to proceed", firstRequestAmount(list));
        sem_post(&mutex);
    }
    else sem_post(&wlist);

    pthread_exit(NULL);
}


void *withdrawerThread(void *threadId)
{
    /*
Withdrawing Customer:// Assume that the local variable withdraw (int) contains the amount to be withdrawn.
wait (mutex);
if (wcount = 0 and balance > withdraw){
    balance := balance – withdraw;
    signal (mutex);
} else { // Either other withdrawal requests are waiting or not enough balance.
    AddEndOf List (LIST, withdraw);
    wcount := wcount + 1;
    signal (mutex);
    wait (wlist); // Start waiting for a deposit
    balance := balance – FirstRequestAmount (LIST); // Withdraw.
    DeleteFirstRequest (LIST); // Remove own request from the waiting list.
    wcount := wcount – 1;
    if (FirstRequestAmount (LIST) ≤ balance) {
        signal (wlist)
    } else {
        signal (mutex);
    }
} // Withdrawal is completed.
    */


    sem_wait(&mutex);
    int withdrawAmount = randomVal();
    if(wcount==0 && balance>withdrawAmount){
        balance -= withdrawAmount;
        printf("\nBalance: %d\t\tPost withdrawal of: %d", balance, withdrawAmount);
    } else{
        add_to_list(list, withdrawAmount, true);
        wcount++;
        sem_post(&mutex);
        sem_wait(&wlist);
        balance -= firstRequestAmount(list);
        deleteFirstRequest(list);
        wcount--;
        if(firstRequestAmount(list)<= balance) {
            sem_post(&wlist);
        } else {
            sem_post(&mutex);
        }
    }

    pthread_exit(NULL);
}

int randomVal() {
    return rand()%1000;
}
