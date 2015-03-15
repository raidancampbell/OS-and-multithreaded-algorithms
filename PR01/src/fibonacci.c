#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "fibonacci.h"

#define get_time time(NULL)

int f1 = 1;
int f2 = 1;
int fib_sum = 0;
int fib_count =  2;


/**
* fork them children
*   have them do some fibbin'
*   everyone prints out some stats
*   then exit/return.
*/
int main(){
	time_t start_s = get_time;
	pid_t child1;
	pid_t child2 = -1;
	child1 = fork();
    //fork the first child
	if(child1<0){
        //I am a sad process.
		perror("ERROR WHILE FORKING FIRST CHILD");
	} else if(child1 == 0) {
		//I am the child process.
        start_s = get_time;
	} else {
		//I am the parent process.
        printf("Parent says fibonacci number 1 is %d\n",f1);
        printf("Parent says fibonacci number 2 is %d\n",f2);
        fflush(stdout);

        //fork the second child
		child2 = fork();
		if(child2<0) perror("ERROR WHILE FORKING SECOND CHILD");
        else if (child2 == 0) start_s = get_time;
	}

    sleep(1);
	if(child1 == 0) fib(1, start_s);
	if(child2 == 0) fib(2, start_s);

    printExitCodes(child1,child2);
    printPID();
    printProcessStats(start_s);
    printCWD();
    printLogin();

	return 0;//parent finishes.
}

/**
* safely prints the current PID
*/
void printPID(){
    pid_t pid;
    if((pid = getpid()) < 0) perror("ERROR WHILE GETTING PID!");
    printf("\nParent has PID %d", pid);
}

/**
* safely prints the current working directory
*/
void printCWD(){
    char dir[1024];
    getcwd(dir, sizeof(dir));
    if(dir == NULL) perror("ERROR WHILE GETTING CURRENT WORKING DIRECTORY");
    printf("CWD is %s\n",dir);
}

/**
* prints the username of the user executing this thread
* This is deprecated, and works most of the time.  Seriously, screw that.
*/
void printLogin(){
    printf("Login: %s\n",getlogin());
    fflush(stdout);
}

/**
* waits for given PIDs to finish, then prints out their exit codes.
*/
void printExitCodes(pid_t child1, pid_t child2){
    int child1Status = -1;
    if(waitpid(child1, &child1Status, 0) < 0) perror("WAITPID FAILED!");//wait for first child to finish
    printf("Child 1 exited with exit code: %d\n", child1Status);
    int child2Status = -1;
    if(waitpid(child2, &child2Status, 0) < 0) perror("WAITPID FAILED!");//wait for second child to finish
    printf("Child 2 exited with exit code: %d\n", child2Status);
    fflush(stdout);
}

/**
* calculates the next fibonacci number (using shared memory)
*   will print out execution time and stats before exiting process.
*/
void fib(int childNumber, time_t start_s){
    pid_t pid  = getpid();
    int iterations = 0;
    for(iterations = 0; iterations < 8; iterations++){
        if(childNumber == 2)sleep(1);
        fib_count++;
        fib_sum = f1 + f2;
        f2 = f1;
        f1 = fib_sum;
        if(fib_count % 2 == childNumber % 2)  {
            printf("Child%d, with PID %d, says fibonacci number %d is %d\n", childNumber, pid, fib_count, fib_sum);
            fflush(stdout);
        }
        if(childNumber == 2) sleep(1);
        if(childNumber == 1) sleep(2);
    }
	if(childNumber == 2) sleep(1);
    printf("\nChild%d has PID %d", childNumber, getpid());
    printProcessStats(start_s);
    fflush(stdout);
    exit(EXIT_SUCCESS);
}

/**
* does: real/effective userIDs
*       real/effective groupIDs
*       PID
*       user CPU time
*       system CPU time
*       wall clock execution time
*/
void printProcessStats(time_t startTime) {
        int i = 0;
        for(i = 0; i < 999999999; i++);
    int realuserid = geteuid(); //cuserid wasn't resolving...
    if(realuserid<0) perror("ERROR WHILE GETTING REAL USER ID");

    int effuserid = getuid();
    if(effuserid<0) perror("ERROR WHILE GETTING EFFECTIVE USER ID");

    int realgroupid = getegid();
    if(realgroupid < 0) perror("ERROR WHILE GETTING REAL GROUP ID");

    int effgroupid = getgid();
    if(effgroupid < 0) perror("ERROR WHILE GETTING EFFECTIVE GROUP ID");

    pid_t pid = getpid();
    if(pid < 0) perror("ERROR WHILE GETTING PID");

    printf("\nProcess with ID %d has:\nReal UID: %d\nEffective UID: %d\nReal GID: %d\nEffective GID: %d\n",
            pid, realuserid, effuserid, realgroupid, effgroupid);
    struct rusage rusage1;
    if(getrusage(RUSAGE_SELF, &rusage1 ) < 0) perror("ERROR WHILE GETTING RESOURCE USAGE");
    printf("Usertime: sec=%ld, usec=%ld\n", (long)(rusage1).ru_utime.tv_sec, (long)(rusage1).ru_utime.tv_usec);
    printf("Systime: sec=%ld, usec=%ld\n", (long)(rusage1).ru_stime.tv_sec, (long)(rusage1).ru_stime.tv_usec);
    printf("Process took %.9f seconds to execute\n", difftime(get_time,startTime));
    time_t currentTime;
    time(&currentTime);
    printf("The current time is: %s\n",ctime(&currentTime));
    fflush(stdout);
}
