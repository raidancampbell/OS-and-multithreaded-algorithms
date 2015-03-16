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

int safesemget(key_t, int, int);
int safeshmget(key_t, size_t , int);
int main();
void makeLeftToRight();
void makeRightToLeft();

struct common {
    int crossing;//number of cars crossing
    int crossed;//number of cars that have crossed in this current direction iteration
    int leftToRightWaiting;//number of rightbound cars waiting
    int rightToLeftWaiting;//number of leftbound cars waiting
    int direction;//enumerated direction as NONE, LEFTTORIGHT, or RIGHTTOLEFT
};