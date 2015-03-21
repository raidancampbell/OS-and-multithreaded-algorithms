//
// Created by Aidan Campbell on 3/15/15.
//
#ifndef _MAINH_
#define _MAINH_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>

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

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

int safesemget();
int safeshmget();
int main();
void makeLeftToRight(struct common*);
void makeRightToLeft(struct common*);
struct common * initializeSharedMemory(int, int);


#endif
