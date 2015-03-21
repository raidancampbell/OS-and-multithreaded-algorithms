#include "main.h"
#include "car.h"
#include <sys/sem.h>

int main(){

    int semid = safesemget();//3 semaphores: mutex, leftbound, rightbound
    int shmid = safeshmget();
    union semun semctlarg;
    unsigned short seminit[NUM_SEMS];
    seminit[0] = 1;//mutex initialized to 1
    seminit[1] = 0;//leftbound initialized to 0
    seminit[2] = 0;//rightbound initialized to 0
    semctlarg.array = seminit;
    int semset = semctl(semid, NUM_SEMS, SETALL, semctlarg);//initialize the semaphore array
    if(semset<0) perror("ERROR ON SETTING SEMAPHORE VALUES!");

    struct common *shared = initializeSharedMemory(shmid, semid);

    //make 12 random cars
    int i = 0;
    for(; i< 12; i++){
        if(rand() % 2 == 0) makeRightToLeft(shared);
        else makeLeftToRight(shared);
    }
    for(i = 0; i<12; i++) wait(0);

    //cleanup
    semctl(semid, NUM_SEMS, IPC_RMID, 0);
    shmctl(shmid, IPC_RMID, 0);

    return EXIT_SUCCESS;
}

int safesemget(){
    int returnVar = semget(IPC_PRIVATE,NUM_SEMS,0666 | IPC_CREAT);
    if(returnVar < 0) perror("ERROR WHILE GETTING SEMID!");
    return returnVar;
}

int safeshmget(){
    int shmid = shmget(IPC_PRIVATE,sizeof(struct common),IPC_CREAT | 0666);
    if(shmid < 0) perror("ERROR WHILE GETTING SHMID!");
    return shmid;
}

void makeLeftToRight(struct common* shared){
    pid_t child = fork();
    if(child < 0){
        perror("Error whild creating Left To Right child process!");
    } else if(child != 0){
        //this is the parent
        return;
    } else {
        car(LEFTTORIGHT, shared);
        exit(EXIT_SUCCESS);

        //this is the child
    }
}

void makeRightToLeft(struct common* shared){
    pid_t child = fork();
    if(child < 0){
        perror("Error whild creating Right To Left child process!");
    } else if(child == 0){
        car(RIGHTTOLEFT, shared);
        exit(EXIT_SUCCESS);
        //this is the child
    } else {
        return;
        //this is the parent
    }
}

struct common* initializeSharedMemory(int shmid, int semid){
    struct common* shared = (struct common *)shmat(shmid, 0, 0);
    shared->crossed = 0;
    shared->crossing = 0;
    shared->direction = NONE;
    shared->leftToRightWaiting = 0;
    shared->rightToLeftWaiting = 0;
    shared->shmkey = shmid;
    shared->semkey = semid;
    return shared;
}