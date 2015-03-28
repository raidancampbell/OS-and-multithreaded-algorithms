//============================================================================
//          THE CODE BELOW WAS HEAVILY RECYCLED FROM THIS SOURCE:
// Name        : posixExample.c
// Author      : Daniel M. Savel
// CaseID      : dxs221
// Description : POSIX Examples, POSIX Threads and POSIX Semaphores
//		 Simple Threaded Readers-Writers Simulation
// Compile CMD : gcc posixExample.c -lpthread
//          WITH SOME OF THE LINKED LIST CODE FROM THIS SOURCE:
//  http://www.thegeekstuff.com/2012/08/c-linked-list-example/
//============================================================================

/*
*R. Aidan Campbell
*   rac158
*   3.28.2015
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>

struct node {
    int val;
    struct node *next;
    int threadID;
};


#define SIGNAL sem_post
#define WAIT sem_wait

//Pointers to methods
void *depositorThread(void *threadId);
void *withdrawerThread(void *threadId);
struct node * create_list(int, int);//initializes a linked list
struct node * add_to_list(int,int);//adds the given int pair to the linked list, bool=true for add to end
int remove_head();//moves the head pointer one back on the linked list

//POSIX Semaphores and shared variables
sem_t wlist, mutex;

int balance = 9001;//initial starting balance
int wcount = 0;//number of withdrawals waiting
int threadIWantToWake = -99;//threadID to wake, used to circumvent the POSIX SIGNAL race condition
int totalWithdraw = 50000000;//a funky exponential distribution of cash throughout the withdrawals/deposits
int totalDeposit = 50000000;
struct node *head = NULL;//top of list
struct node *curr = NULL;//bottom of list

struct threadInfo {int threadId;};

int main(void) {
    srand((unsigned)time(NULL));    //Seed the Random Number Generator
    //initialize them variables
    int depositorCount = 50;
    int withdrawerCount = 50;
    int depositorsRemaining = depositorCount;
    int withdrawerRemaining = withdrawerCount;
    printf("Counts initialized. %d Depositors\t\t%d Withdrawers", depositorCount, withdrawerCount);

    struct threadInfo depositorIDs[depositorCount];
    struct threadInfo withdrawerIDs[withdrawerCount];

    pthread_t depositors[depositorCount];
    pthread_t withdrawers[withdrawerCount];
    pthread_attr_t attr;

    //Creating a set of attributes to send to the threads
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //Initializing the WRT and MUTEX semaphores
    sem_init(&wlist, 0, 0);//initialized to 0
    sem_init(&mutex, 0, 1);//initialized to 1

    //Spawn threads
    while(depositorsRemaining+withdrawerRemaining >0){
        if(usleep(1000)<0) perror("usleep");
        int rnd = rand()%2;
        if(rnd==0 && depositorsRemaining>0){
            depositorsRemaining--;
            int i = depositorCount - depositorsRemaining;
            depositorIDs[i].threadId = i;
            int result = pthread_create(&depositors[i], &attr, depositorThread, (void *)&depositorIDs[i]);
            if(result < 0) {
                perror("Thread Creation: Depositor");
                exit(EXIT_FAILURE);
            }
        } else if(rnd==1 && withdrawerRemaining>0){
            withdrawerRemaining--;
            int i = withdrawerCount - withdrawerRemaining;
            withdrawerIDs[i].threadId = i;
            int result = pthread_create(&withdrawers[i], &attr, withdrawerThread, (void *)&withdrawerIDs[i]);
            if(result < 0) {
                perror("Thread Creation: Withdrawer");
                exit(EXIT_FAILURE);
            }
        }

    }
    int i;
    void *status;
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
    WAIT(&mutex);
    int depositAmount = rand() % (totalDeposit/2);
    totalDeposit -= depositAmount;
    if(totalDeposit<0){
        perror("total deposit is below zero!");
    }
    usleep(100);
    balance = balance + depositAmount;
    printf("\nDepositor: %d\tBalance: %d\t\tPost deposit of: %d", myThreadID, balance, depositAmount);
    if(wcount==0) {
        SIGNAL(&mutex);
    } else if (head != NULL && head->val > balance) {
        SIGNAL(&mutex);
    } else {
        threadIWantToWake = head->threadID;
        //printf("\nI want to wake thread %d so they can withdraw %d", threadIWantToWake, head->val);
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
    WAIT(&mutex);
    int withdrawAmount = rand() % (totalWithdraw/2);
    totalWithdraw -= withdrawAmount;

    if(totalWithdraw<0){
        perror("totalwithdraw is below 0");
    }

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
        add_to_list(withdrawAmount, myThreadID); //adds withdrawAmount to the end of the list
        wcount = wcount + 1;
        SIGNAL(&mutex);
        while (1) {
            WAIT(&wlist);
            usleep(10);
            if(myThreadID == threadIWantToWake) {
                printf("\nI'm the thread you wanted to awake, I say %d = %d", myThreadID, threadIWantToWake);
                printf("\nInclusive of me, there are %d threads in line", wcount);
                threadIWantToWake = -99;
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
            printf("\nWithdrawer: %d\t\tI want to wake thread %d", myThreadID, threadIWantToWake);
            SIGNAL(&wlist);
        } else {
            SIGNAL(&mutex);
        }
    }
    pthread_exit(NULL);
}

//below here is things adapted from
//  http://www.thegeekstuff.com/2012/08/c-linked-list-example/
//I removed unused code, added the remove_head, and fixed some NPE things


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

struct node * add_to_list(int val,int threadID) {
    usleep(10);
    if(head == NULL) {
        return (create_list(val, threadID));
    }
    if(curr == NULL) {
        perror("CURR WAS NULL!");
        curr = head;
    }

    printf("\nAdding node to end of list with value [%d]",val);

    struct node *ptr = (struct node *)malloc(sizeof(struct node));
    if(ptr == NULL) {
        printf("\nNode creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->threadID = threadID;
    ptr->next = NULL;

    if(curr == NULL){
        perror("CURR WAS NULL!");
        curr = head;
    }
    curr->next = ptr;
    curr = ptr;
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