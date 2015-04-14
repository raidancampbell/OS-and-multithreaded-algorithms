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

#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "MailBox.h"


user* getUser();
int callStart(user*, CLIENT*, int*);
int callInsertMessage(user, char*, CLIENT*, int*);
char* callListAllMessages(user*, CLIENT*);
char* callRetrieveMessage(user,int, CLIENT*);
int* callDeleteMessage(user*, int, CLIENT*);
int callStop(user*, CLIENT*, int*);

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
    char* string = callListAllMessages(myUser, client);
    printf("\nclient: received messages.  Printing...\n%s", string);

    //RETRIEVE_MESSAGE
    string  = callRetrieveMessage(*myUser, 0, client);
    printf("\nreceived message \n%s", string);

    //DELETE_MESSAGE
    return_value  = callDeleteMessage(myUser, 0, client);
    if(!*return_value) return 0;

    return_value = (int*) callStop(myUser, client, return_value);
    return (int) return_value;
}

user* getUser(){
    char myHost[81];
    myHost[80] = '\0';
    if(gethostname(myHost, sizeof(myHost)) < 0) perror("HOSTNAME");
    return &(user) {.hostname=myHost, .uuid=42};
}

int callStart(user* myUser, CLIENT* client, int* return_value){
    printf("\nclient : initializing START, with username: %s%d.", myUser->hostname, myUser->uuid);
    return_value = start_1(myUser, client);
    if (*return_value)  printf("\nclient : START successful.\n");
    else  printf("\nclient : START failed.\n");
    return *return_value;
}

int callInsertMessage(user destinationUser, char* message, CLIENT* client, int* return_value){
    printf("\nclient : Inserting Message to username: %s%d.\n", destinationUser.hostname, destinationUser.uuid);
    return_value = insert_message_1(&((insert_message_params){.given_user=destinationUser, .message=message}), client);
    if (*return_value)  printf("\nclient : insert_message successful.\n");
    else  printf("\nclient : insert_message failed.\n");
    return *return_value;
}

char* callListAllMessages(user* myUser, CLIENT* client){
    printf("\nclient : listing all messages");
    message_block * return_value = list_all_messages_1(myUser, client);
    if(return_value == NULL) {
        printf("\nclient : list_all_messages failed.");
        return NULL;
    }
    return return_value->data;
}

int* callDeleteMessage(user* myUser, int messageID, CLIENT* client){
    printf("\nclient : deleting message %d", messageID);
    delete_message_params params;
    params.message_number = messageID;
    params.given_user = *myUser;
    return delete_message_1(&params, client);
}

char* callRetrieveMessage(user myUser, int messageID,  CLIENT* client){
    printf("\nclient : retrieving message %d", messageID);
    retrieve_message_params params;
    params.given_user = myUser;
    params.message_number = messageID;
    char * return_value = *retrieve_message_1(&params, client);
    return return_value;
}

int callStop(user* myUser, CLIENT* client, int* return_value){
    printf("\nclient : quitting");
    return_value = quit_1(myUser, client);
    if (*return_value)  printf("\nclient : quit successful.\n");
    else  printf("\nclient : quit failed.\n");
    return *return_value;
}