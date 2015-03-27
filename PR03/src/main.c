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
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

struct node {
    int val;
    struct node *next;
    int threadID;
};

//#include "LL.h"

#define SIGNAL sem_post
#define WAIT sem_wait

//Pointers to methods
void *depositorThread(void *threadId);
void *withdrawerThread(void *threadId);
int randomVal();
struct node * create_list(int, int);
struct node * add_to_list(int,int, bool);
int remove_head();

//POSIX Semaphores and shared variables
sem_t wlist, mutex;

int balance = 500;//initial starting balance
int wcount = 0;
int threadIWantToWake = 0;
struct node *head = NULL;
struct node *curr = NULL;

struct threadInfo {int threadId;};

int main(void) {
    srand((unsigned)time(NULL));    //Seed the Random Number Generator
    int depositorCount = randomVal()%7;
    int depositorsRemaining = depositorCount;
    int withdrawerCount = randomVal()%10;
    int withdrawerRemaining = withdrawerCount;
    printf("Counts initialized. %d Depositors\t\t%d Withdrawers", depositorCount, withdrawerCount);
    struct threadInfo depositorIDs[depositorCount];
    struct threadInfo withdrawerIDs[withdrawerCount];
    pthread_t depositors[depositorCount];
    pthread_t withdrawers[withdrawerCount];
    pthread_attr_t attr;
    void *status;

    //Creating a set of attributes to send to the threads
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //Initializing the WRT and MUTEX semaphores
    sem_init(&wlist, 0, 0);
    sem_init(&mutex, 0, 1);
    //Spawn threads
    while(depositorsRemaining+withdrawerRemaining >0){
        if(usleep(1000)<0) perror("usleep");
        int rnd = rand()%2;
        if(rnd==0 && depositorsRemaining>0){
            depositorsRemaining--;
            int i = depositorCount - depositorsRemaining;
            depositorIDs[i].threadId = i;
            int result = pthread_create(&depositors[i], &attr, depositorThread, (void *) &depositorIDs[i]);
            if(result < 0) {
                perror("Thread Creation: Depositor");
                exit(EXIT_FAILURE);
            }
        } else if(rnd==1 && withdrawerRemaining>0){
            withdrawerRemaining--;
            int i = withdrawerCount = withdrawerRemaining;
            int result = pthread_create(&withdrawers[i], &attr, withdrawerThread, (void *) &withdrawerIDs[i]);
            if(result < 0) {
                perror("Thread Creation: Withdrawer");
                exit(EXIT_FAILURE);
            }
        }
    }
    int i;
    //Wait for all the threads to finish
    for(i = 0; i < depositorCount; i++) {
        int result = pthread_join(depositors[i], &status);
        if(result < 0) {
            perror("Thread Join: Depositor");
            exit(EXIT_FAILURE);
        }
    }

    for(i = 0; i < withdrawerCount; i++) {
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
    struct threadInfo * info;
    info = (struct threadInfo *) threadId;
    int myThreadID = info->threadId;
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

    //Depositor Entry
    int depositAmount = randomVal();
    WAIT(&mutex);
    usleep(100);
    balance = balance + depositAmount;
    printf("\nDepositor: %d\tBalance: %d\t\tPost deposit of: %d", myThreadID, balance, depositAmount);
    if(wcount==0) {
        SIGNAL(&mutex);
    } else if (head != NULL && head->val > balance) {
        SIGNAL(&mutex);
    } else {
        threadIWantToWake = head->threadID;
        printf("\nI want to wake thread %d", threadIWantToWake);
        printf("\nThat deposit allowed the waiting withdrawal of: %d to proceed", head->val);
        SIGNAL(&wlist);
    }


    pthread_exit(NULL);
}


void *withdrawerThread(void *threadId)
{
    struct threadInfo * info;
    info = (struct threadInfo *) threadId;
    int myThreadID = info->threadId;
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
    int withdrawAmount = randomVal()%700;//slightly less than deposit amount, to help things move forward
    if(withdrawAmount < 0){
        perror("WITHDRAW AMOUNT LESS THAN 0");
        exit(EXIT_FAILURE);
    }
    WAIT(&mutex);
    usleep(10);
    if(wcount==0 && balance>withdrawAmount){
        balance = balance - withdrawAmount;
        if(balance<0){
            perror("A Balance of less than 0 was created!");
            printf("\n\tnegative balance from above error: %d", balance);
        }
        printf("\nWithdrawer: %d\t\tBalance: %d\t\tPost withdrawal of: %d", myThreadID, balance, withdrawAmount);
        SIGNAL(&mutex);
    } else {
        printf("\nWithdrawer: %d\t\tBalance: %d\t\tPre overdraw/wait of: %d", myThreadID, balance, withdrawAmount);
        /*head =*/ add_to_list(withdrawAmount, myThreadID, true); //adds withdrawAmount to the end of the list
        wcount = wcount + 1;
        SIGNAL(&mutex);
        while (1) {
            WAIT(&wlist);
            usleep(10);
            if(myThreadID == threadIWantToWake) {
                printf("\nI'm the thread you wanted to awake, I say %d = %d", myThreadID, threadIWantToWake);
                printf("\nInclusive of me, there are %d threads in line", wcount);
                break;
            }
        }
        balance = balance - withdrawAmount;
        if(balance<0){
            perror("A Balance of less than 0 was created!");
            printf("\n\tnegative balance from above error: %d", balance);
        }
        printf("\nWithdrawer: %d\t\tBalance: %d\t\tPost delayed withdrawal of: %d", myThreadID, balance, withdrawAmount);
        remove_head();
        wcount--;
        if(wcount != 0 && head != NULL && head->val <= balance) {
            threadIWantToWake = head->threadID;
            printf("\nI want to wake thread %d", threadIWantToWake);
            SIGNAL(&wlist);
        } else {
            SIGNAL(&mutex);
        }
    }
    pthread_exit(NULL);
}

int randomVal() {
    return rand()%1000;
}

struct node * create_list(int val, int threadID) {
    printf("\ncreating list with headnode as [%d]",val);
    struct node *ptr = (struct node *)malloc(sizeof(struct node));
    if(ptr == NULL) {
        printf("\nNode creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->threadID = threadID;
    ptr->next = NULL;

    head = ptr;
    curr = ptr;
    if(head == NULL) printf("\nNode creation (that I know about) failed");
    return head;
}

struct node * add_to_list(int val,int threadID , bool add_to_end) {
    usleep(10);
    if(NULL == head) {
        return (create_list(val, threadID));
    }
    if(NULL == curr) {
        perror("CURR WAS NULL!");
        curr = head;
    }

    if(add_to_end)  printf("\nAdding node to end of list with value [%d]",val);
    else            printf("\nAdding node to beginning of list with value [%d]",val);

    struct node *ptr = (struct node *)malloc(sizeof(struct node));
    if(NULL == ptr) {
        printf("\nNode creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->threadID = threadID;
    ptr->next = NULL;

    if(add_to_end) {
        if(curr == NULL){
            perror("CURR WAS NULL!");
            curr = head;
        }
        curr->next = ptr;
        curr = ptr;
    }
    else {
        ptr->next = head;
        head = ptr;
    }
    return ptr;
}

int remove_head()
{
    if(head == NULL) return -1;
    head = head->next;
    if(head == NULL){
        printf("\nList emptied.");
    }
    return 0;
}