//
// Created by Aidan Campbell on 3/15/15.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NONE 0
#define LEFTTORIGHT 1
#define RIGHTTOLEFT 2
#define NUM_SEMS 3
#define KEY 32766
#define WAIT -1
#define SIGNAL 1
#define MUTEX 0
#define LEFTBOUND 1
#define RIGHTBOUND 2

struct common {
    int crossing;//number of cars crossing
    int crossed;//number of cars that have crossed in this current direction iteration
    int leftToRightWaiting;//number of rightbound cars waiting
    int rightToLeftWaiting;//number of leftbound cars waiting
    int direction;//enumerated direction as NONE, LEFTTORIGHT, or RIGHTTOLEFT
    int semkey;
    int shmkey;
};

int safesemget(key_t, int, int);
int safeshmget();
int main();
void makeLeftToRight(struct common*);
void makeRightToLeft(struct common*);
struct common * initializeSharedMemory(int, int);

