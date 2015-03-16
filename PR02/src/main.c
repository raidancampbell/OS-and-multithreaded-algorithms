#include "main.h"

int main(){

    int semid = safesemget(KEY,NUM_SEMS,0666 | IPC_CREAT);//3 semaphores: mutex, leftbound, rightbound
    int shmid = safeshmget(KEY+1,0,0);
    union semun semctlarg;
    unsigned short seminit[NUM_SEMS];
    seminit[0] = 1;//mutex initialized to 1
    seminit[1] = 0;//leftbound initialized to 0
    seminit[2] = 0;//rightbound initialized to 0
    semctlarg.array = seminit;
    semctl(semid, NUM_SEMS, SETALL, semctlarg);//initialize the semaphore array


    struct common *shared = (struct common *)shmat(shmid, 0, 0);
    shared->crossed = 0;
    shared->crossing = 0;
    shared->direction = NONE;
    shared->leftToRightWaiting = 0;
    shared->rightToLeftWaiting = 0;

    makeLeftToRight();
    makeRightToLeft();


    wait(0);//wait for the children to die
    wait(0);
    //cleanup
    semctl(semid, NUM_SEMS, IPC_RMID, 0);
    shmctl(shmid, IPC_RMID, 0);
}

int safesemget(key_t k, int i1, int i2){
    int returnVar = semget(k,i1,i2);
    if(returnVar < 0) perror("ERROR WHILE GETTING SEMID!");
    return returnVar;
}

int safeshmget(key_t k, size_t i1, int i2){
    int shmid = shmget(k,i1,i2);
    if(shmid < 0) perror("ERROR WHILE GETTING SHMID!");
    return shmid;
}


void makeLeftToRight(){
    pid_t child = fork();
    if(child < 0){
        perror("Error whild creating Left To Right child process!");
    } else if(child == 0){
        //this is the parent
        return;
    } else {
        execl();
        exit(EXIT_SUCCESS);
        //this is the child
    }
}

void makeRightToLeft(){
    pid_t child = fork();
    if(child < 0){
        perror("Error whild creating Right To Left child process!");
    } else if(child == 0){
        //this is the parent
        return;
    } else {
        execl();
        exit(EXIT_SUCCESS);
        //this is the child
    }
}