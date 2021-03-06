#include <time.h>
#include "car.h"
#include <sys/sem.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

void car(int direction, struct common* shared){
    //safety check for a valid input
    if(direction != LEFTTORIGHT && direction != RIGHTTOLEFT) perror("ERROR: INVALID ARGUMENT GIVEN TO CAR PROCESS");

    //branch off into the correct car type
    if(direction == LEFTTORIGHT) lefttoright(shared);
    else righttoleft(shared);
}

void lefttoright(struct common *shared){
    printf("\n%d:\tlefttoright car created. Executing", getpid());
    fflush(stdout);
    struct sembuf wait_mutex = {MUTEX, WAIT, 0};
    struct sembuf signal_mutex = {MUTEX, SIGNAL, 0};
    struct sembuf wait_rightbound = {RIGHTBOUND, WAIT, 0};
    struct sembuf signal_leftbound = {LEFTBOUND, SIGNAL, 0};
    struct sembuf signal_rightbound = {RIGHTBOUND, SIGNAL, 0};
    /*
    * wait(mutex);
    if ((XingDirection=EastBnd or XingDirection=None) and XingCount<4 and (XedCount+XingCount)<5))
    {XingDirection:=EastBnd; XingCount++; signal(mutex)} else {EastBndWaitCount++; signal(mutex); wait(EastBound);
    EastBndWaitCount−−; XingCount++; XingDirection:=EastBnd; signal(mutex)}
    CROSS!!
    wait(mutex); XedCount++; XingCount−−;
    if (EastBndWaitCount≠0 and ((XedCount+XingCount)<5) or WestBndWaitCount=0)) signal(EastBound)
    else if (XingCount=0 and WestBndWaitCount≠0 and (EastBndWaitCount=0 or (XedCount+XingCount)≥5)) {XingDirection:=WestBnd; XedCount:=0; signal(WestBound)}
    else if (XingCount=0 and EastBndWaitCount=0 and WestBndWaitCount=0) {XingDirection:=None; XedCount:=0; signal(mutex)}
    else signal(mutex)
    */
    waitOrSignal(shared->semkey, wait_mutex);
    if((shared->direction == LEFTTORIGHT || shared->direction == NONE) && shared->crossing < 4 && (shared->crossed + shared->crossing < 5)){
        shared->direction = LEFTTORIGHT;
        shared->crossing++;
        waitOrSignal(shared->semkey, signal_mutex);
    } else {
        shared->leftToRightWaiting++;
        waitOrSignal(shared->semkey, signal_mutex);
        waitOrSignal(shared->semkey, wait_rightbound);
        shared->leftToRightWaiting--;
        shared->crossing++;
        shared->direction = LEFTTORIGHT;
        waitOrSignal(shared->semkey, signal_mutex);
    }
    printf("\n%d\tCars on bridge: %d\tCars crossed: %d\tleftToRightWaiting: %d\trightToLeftWaiting: %d",getpid(),shared->crossing, shared->crossed, shared->leftToRightWaiting, shared->rightToLeftWaiting);
    cross(LEFTTORIGHT);//actually takes some time
    waitOrSignal(shared->semkey, wait_mutex);
    shared->crossing--;
    shared->crossed++;
    if(shared->leftToRightWaiting != 0 && ((shared->crossed+shared->crossing)<5 || shared->rightToLeftWaiting==0))  waitOrSignal(shared->semkey, signal_rightbound);
    else if (shared->crossing==0 && shared->rightToLeftWaiting !=0 && (shared->leftToRightWaiting==0 || (shared->crossing+shared->crossed)>=5)){
        printf("\n%d\tDirection was left to right, I switched it to right to left",getpid());
        shared->direction = RIGHTTOLEFT;
        shared->crossed = 0;
        waitOrSignal(shared->semkey, signal_leftbound);
    } else if(shared->crossing==0 && shared->leftToRightWaiting ==0 && shared->rightToLeftWaiting==0){
        shared->direction = NONE;
        shared->crossed = 0;
        waitOrSignal(shared->semkey, signal_mutex);
    } else {
        waitOrSignal(shared->semkey, signal_mutex);
    }
}

void righttoleft(struct common *shared){
    printf("\n\n%d:\trighttoleft car created. Executing", getpid());
    fflush(stdout);
    struct sembuf wait_mutex = {MUTEX, WAIT, 0};
    struct sembuf signal_mutex = {MUTEX, SIGNAL, 0};
    struct sembuf signal_leftbound = {LEFTBOUND, SIGNAL, 0};
    struct sembuf signal_rightbound = {RIGHTBOUND, SIGNAL, 0};
    struct sembuf wait_leftbound = {LEFTBOUND, WAIT, 0};
    /*
    * wait(mutex);
    if ((XingDirection=WestBnd or XingDirection=None) and XingCount<4 and (XedCount+XingCount)<5))
    {XingDirection:=WestBnd; XingCount++; signal(mutex)} else {WestBndWaitCount++; signal(mutex); wait(WestBound);
    WestBndWaitCount−−; XingCount++; XingDirection:=WestBnd; signal(mutex)}
    CROSS!!
    wait(mutex);
    XedCount++;
    XingCount−−;
    if (WestBndWaitCount≠0 and ((XedCount+XingCount)<5) or EastBndWaitCount=0))
    signal(WestBound)
    else if (XingCount=0 and EastBndWaitCount≠0 and (WestBndWaitCount=0 or (XedCount+XingCount)≥5))
    {XingDirection:=EastBnd; XedCount:=0; signal(EastBound)}
    else if (XingCount=0 and EastBndWaitCount=0 and WestBndWaitCount=0)
    {XingDirection:=None; XedCount:=0; signal(mutex)} else signal(mutex)
    }
    */
    waitOrSignal(shared->semkey, wait_mutex);
    if((shared->direction==RIGHTTOLEFT ||shared->direction==NONE) && shared->crossing<4 && (shared->crossed+shared->crossing)<5){
        shared->direction = RIGHTTOLEFT;
        shared->crossing++;
        waitOrSignal(shared->semkey, signal_mutex);
    } else {
        shared->rightToLeftWaiting++;
        waitOrSignal(shared->semkey, signal_mutex);
        waitOrSignal(shared->semkey, wait_leftbound);
        shared->rightToLeftWaiting--;
        shared->crossing++;
        shared->direction = RIGHTTOLEFT;
        waitOrSignal(shared->semkey, signal_mutex);
    }
    printf("\n%d\tCars on bridge: %d\tCars crossed: %d\tleftToRightWaiting: %d\trightToLeftWaiting: %d",getpid(),shared->crossing, shared->crossed, shared->leftToRightWaiting, shared->rightToLeftWaiting);
    cross(RIGHTTOLEFT);
    waitOrSignal(shared->semkey, wait_mutex);
    shared->crossed++;
    shared->crossing--;
    if(shared->rightToLeftWaiting!=0 && ((shared->crossed+shared->crossing)<5 || shared->leftToRightWaiting==0)) waitOrSignal(shared->semkey, signal_leftbound);
    else if(shared->crossing==0 && shared->leftToRightWaiting!=0 && (shared->rightToLeftWaiting==0 || (shared->crossing+shared->crossed)>=5)){
        printf("\n%d\tDirection was right to left, I switched it to left to right",getpid());
        shared->direction = LEFTTORIGHT;
        shared->crossed = 0;
        waitOrSignal(shared->semkey, signal_rightbound);
    } else if(shared->crossing==0 && shared->rightToLeftWaiting==0 && shared->leftToRightWaiting==0){
        shared->direction = NONE;
        shared->crossed = 0;
        waitOrSignal(shared->semkey, signal_mutex);
    } else {
        waitOrSignal(shared->semkey, signal_mutex);
    }
}

void waitOrSignal(int semid, struct sembuf operation){
    if(semop(semid, &operation, 1) < 0) {
        perror("ERROR ON SEMAPHORE WAIT/SIGNAL!");
        printf("\tSEMID: %d\n", semid);
        _exit(EXIT_FAILURE);
    }
}

void cross(int direction){
    if(direction==LEFTTORIGHT)printf("\nPID: %d IS CROSSING LEFT TO RIGHT NOW", getpid());
    else printf("\nPID: %d IS CROSSING RIGHT TO LEFT NOW", getpid());
    struct timespec sleepValue = {0};
    sleepValue.tv_nsec = 500 * 1000000;
    nanosleep(&sleepValue, NULL);
    printf("\nPID: %d HAS CROSSED NOW", getpid());
}
