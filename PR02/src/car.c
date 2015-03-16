#include "main.h"

int main(int direction){

    if(direction != LEFTTORIGHT && direction != RIGHTTOLEFT) {
        perror("ERROR: INVALID ARGUMENT GIVEN TO CAR PROCESS");
    }
    int semid, shmid;
    struct common *shared;
    int mypid = getpid();

    semid = semget(KEY, NUM_SEMS, 0777);
    shmid = shmget(KEY + 1, 0, 0);
    shared = (struct common *)shmat(shmid, 0, 0);
}