//
// Created by Aidan Campbell on 4/10/15.
/*Server has following commands
    START(user)
    QUIT(user)
    RETRIEVE_MESSAGE(user, message number)
    INSERT_MESSAGE(user, message)
    LIST_ALL_MESSAGES(user)
    DELETE_MESSAGE(user, message number)

    testing will be:
    START
    INSERT_MESSAGE
    LIST_ALL_MESSAGES
    RETRIEVE_MESSAGE
    DELETE_MESSAGE
    QUIT
*/
//
#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "MailBox.h"


user* getUser();
int callStart(user*, CLIENT*, int*);
int callInsertMessage(user, char*, CLIENT*, int*);

int main(int argc, char *argv[]) {
    CLIENT         *client;
    int            *return_value;
    user           *myUser;
    char           *server;
    /*
        We must specify a host on which to run.  We will get the host name
        from the command line as argument 1.
     */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s host_name\n", *argv);
        exit(1);
    }
    server = argv[1];
    myUser = getUser();
    /* Generate the client handle to call the server */
    if ((client=clnt_create(server,DISPLAY_PRG, DISPLAY_VER, "tcp")) == (CLIENT*) NULL) {
        clnt_pcreateerror(server);
        exit(2);
    }
    //START
    return_value =  (int*) callStart(myUser, client, return_value);
    if(!*return_value) return 0;
    sleep(10u);/*I need time for both clients to be up, before talking to the server*/

    //INSERT_MESSAGE
    user destinationUser = (user) {.hostname="eecslinab3", .uuid=42};
    return_value = (int*) callInsertMessage(destinationUser, "someone said hello!", client, return_value);
    if(!*return_value) return 0;
    sleep(5u);

    //LIST_ALL_MESSAGES


    return (int) return_value;
}


user* getUser(){
    char myHost[81];
    myHost[80] = '\0';
    if(gethostname(myHost, sizeof(myHost)) < 0) perror("HOSTNAME");
    return &(user) {.hostname=myHost, .uuid=42};
}



int callStart(user* myUser, CLIENT* client, int* return_value){
    printf("client : initializing START, with username: %s%d.\n", myUser->hostname, myUser->uuid);
    return_value = start_1((void *) &myUser, client);
    if (*return_value)  printf("client : START successful.\n");
    else  printf("client : START failed.\n");
    return (int) return_value;
}

int callInsertMessage(user destinationUser, char* message, CLIENT* client, int* return_value){
    printf("client : Inserting Message to username: %s%d.\n", destinationUser.hostname, destinationUser.uuid);
    return_value = insert_message_1(&((insert_message_params){.given_user=destinationUser, .message=message}), return_value);
    if (*return_value)  printf("client : insert_message successful.\n");
    else  printf("client : insert_message failed.\n");
    return (int) return_value;
}