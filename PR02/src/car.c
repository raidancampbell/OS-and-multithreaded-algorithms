#include <stdio.h>
#include <time.h>
#include "main.h"
#include "car.h"
#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds
const long INTERVAL_MS = 500 * NANO_SECOND_MULTIPLIER;

void car(int direction, struct common shared){

    if(direction != LEFTTORIGHT && direction != RIGHTTOLEFT) perror("ERROR: INVALID ARGUMENT GIVEN TO CAR PROCESS");

    int semid, shmid;
    int mypid = getpid();
    shmid = shmget(KEY + 1, 0, 0);
    //shared = (struct common *)shmat(shmid, 0, 0);
    semid = shared.semkey;

    struct sembuf wait_leftbound = {LEFTBOUND, WAIT, 0};
    struct sembuf signal_leftbound = {LEFTBOUND, SIGNAL, 0};
    struct sembuf wait_rightbound = {RIGHTBOUND, WAIT, 0};
    struct sembuf signal_rightbound = {RIGHTBOUND, SIGNAL, 0};

    if(direction == LEFTTORIGHT) lefttoright(&shared);
    else righttoleft(&shared);
}

void lefttoright(struct common *shared){
    struct sembuf wait_mutex = {MUTEX, WAIT, 0};
    struct sembuf signal_mutex = {MUTEX, SIGNAL, 0};
    struct sembuf wait_leftbound = {LEFTBOUND, WAIT, 0};
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
        waitOrSignal(shared->semkey, wait_leftbound);
        shared->leftToRightWaiting--;
        shared->crossing++;
        shared->direction = LEFTTORIGHT;
        waitOrSignal(shared->semkey, signal_mutex);
    }
    cross();//actually takes some time
    waitOrSignal(shared->semkey, wait_mutex);
    shared->crossing--;
    shared->crossed++;
    if(shared->leftToRightWaiting != 0 && ((shared->crossed+shared->crossing)<5 || shared->rightToLeftWaiting==0))
        waitOrSignal(shared->semkey, signal_leftbound);
    else if (shared->crossing==0 && shared->rightToLeftWaiting !=0 && (shared->leftToRightWaiting==0 || (shared->crossing+shared->crossed)>=5)){
        shared->direction = RIGHTTOLEFT;
        shared->crossed = 0;
        waitOrSignal(shared->semkey, signal_rightbound);
    } else if(shared->crossing==0 && shared->leftToRightWaiting ==0 && shared->rightToLeftWaiting==0){
        shared->direction = NONE;
        shared->crossed = 0;
        waitOrSignal(shared->semkey, signal_mutex);
    } else {
        waitOrSignal(shared->semkey, signal_mutex);
    }
}

void righttoleft(struct common *shared){

    struct sembuf wait_mutex = {MUTEX, WAIT, 0};
    struct sembuf signal_mutex = {MUTEX, SIGNAL, 0};
    struct sembuf wait_rightbound = {RIGHTBOUND, WAIT, 0};
    struct sembuf signal_leftbound = {LEFTBOUND, SIGNAL, 0};
    struct sembuf signal_rightbound = {RIGHTBOUND, SIGNAL, 0};
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
        waitOrSignal(shared->semkey, wait_rightbound);
        shared->rightToLeftWaiting--;
        shared->crossing++;
        shared->direction = RIGHTTOLEFT;
        waitOrSignal(shared->semkey, signal_mutex);
    }
    cross();
    waitOrSignal(shared->semkey, wait_mutex);
    shared->crossed++;
    shared.crossing--;
    if(shared->rightToLeftWaiting!=0 && ((shared->crossed+shared->crossing)<5 || shared->leftToRightWaiting==0))
        waitOrSignal(shared->semkey, signal_rightbound);
    else if(shared->crossing==0 && shared->leftToRightWaiting!=0 && (shared->rightToLeftWaiting==0 || (shared->crossing+shared->crossed)>=5)){
        shared->direction = LEFTTORIGHT;
        shared->crossed = 0;
        waitOrSignal(shared->semkey, signal_leftbound);
    } else if(shared.crossing==0 && shared->rightToLeftWaiting==0 && shared->rightToLeftWaiting==0){
        shared->direction = NONE;
        shared->crossed = 0;
        waitOrSignal(shared->semkey, signal_mutex);
    } else {
        waitOrSignal(shared->semkey, signal_mutex);
    }
}

void waitOrSignal(int semid, struct sembuf operation){
    if(semop(semid, &operation, 1) < 0) {
        perror("ERROR ON SEMAPHORE WAIT/SIGNAL");
        _exit(EXIT_FAILURE);
    }
}

void cross(){
    timespec sleepValue = {0};
    sleepValue.tv_nsec = INTERVAL_MS;
    nanosleep(&sleepValue, NULL);
}