#include <sys/sem.h>
#include "main.h"
void car(int, struct common*);
void lefttoright(struct common*);
void righttoleft(struct common*);
void waitOrSignal(int, struct sembuf);
void cross();
