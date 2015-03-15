#include "main.h"

int main(){
    long key = 32766;
    int semid = safesemid(key,3,0777);//3 semaphores: mutex, leftbound, rightbound
    int shmid = safeshmget(key,0,0);

    struct common *shared = (struct common *)shmat(shmid, 0, 0);

    makeLeftToRight();
}

int safesemid(key_t k, int i1, int i2){
    int returnVar = semid(k,i1,i2);
    if(returnVar < 0) perror("ERROR WHILE GETTING SEMID!");
    return returnVar;
}

int safeshmget(key_t k, int i1, int i2){
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