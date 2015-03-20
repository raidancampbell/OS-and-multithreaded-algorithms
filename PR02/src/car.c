#include "main.h"
#include "car.h"

int main(int direction){

    if(direction != LEFTTORIGHT && direction != RIGHTTOLEFT) {
        perror("ERROR: INVALID ARGUMENT GIVEN TO CAR PROCESS");
    }
    int semid, shmid;
    struct common *shared;
    int mypid = getpid();
    shmid = shmget(KEY + 1, 0, 0);
    shared = (struct common *)shmat(shmid, 0, 0);
    semid = shared->semkey;

    struct sembuf wait_mutex = {MUTEX, WAIT, 0}; //for wait(mutex)
    struct sembuf signal_mutex = {MUTEX, SIGNAL, 0}; //for signal(mutex)
    struct sembuf wait_leftbound = {LEFTBOUND, WAIT, 0}; //for wait(empty)
    struct sembuf signal_leftbound = {LEFTBOUND, SIGNAL, 0}; //for wait(empty)
    struct sembuf wait_rightbound = {RIGHTBOUND, WAIT, 0}; //for signal(full)
    struct sembuf signal_rightbound = {RIGHTBOUND, SIGNAL, 0}; //for signal(full)
}

void lefttoright(){

}

void righttoleft(){

}

void waitOrSignal(int semid, struct sembuf operation){
    if(semop(semid, &operation, 1) < 0) {
        perror("ERROR ON SEMAPHORE WAIT/SIGNAL");
        _exit(EXIT_FAILURE);
    }
}