//
// Created by Aidan Campbell on 4/10/15.
//Tekin says "easiest assignment"
//I say "no."
//

#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MailBox.h"

struct message {
    char* text;
};
struct userMailBox {
    user user1;
    struct message mail[20];
};
struct user users[10];
struct userMailBox mailboxes[10];
int getIndexFromUser(user *);
int initFlag = 0;

/* used for clearing and initializing mailboxes*/
struct message emptyMessage(){
    char empty = ' ';
    return (struct message){.text=&empty};
}

//only run once.
//set each user's uuid to 0, and set the mailboxes to be full of empty messages
void init(){
    if(initFlag>0) return;
    initFlag++;

    for(int i = 0; i < sizeof(users) / sizeof(struct user); i++) {
        users[i].uuid = 0;
	struct userMailBox temp = {.user1=NULL,.mail=NULL};
        mailboxes[i] = temp;
    }
}

//get the uninitialized user with the lowest index, -1 if not found.
int getFreeUser(){
    init();
    int i;
    for(i = 0; i < sizeof(users) / sizeof(struct user); i++) {
        if(users[i].uuid == 0) return i;
    }
    return -1;
}

/*START function.  It creates a user in the system */
int * start_1_svc(user * myUser, struct svc_req * req) {
    init();
    int * USER_ALREADY_EXISTS = malloc(sizeof(int));
    int why = -1;
    USER_ALREADY_EXISTS = &why;
    int * USER_CREATE_SUCCESS = malloc(sizeof(int));
    why = 1;
    USER_CREATE_SUCCESS = &why;
    if(getIndexFromUser(myUser) >= 0) return USER_ALREADY_EXISTS;//user already started
    int i = getFreeUser();

    users[i] = *myUser;
    struct userMailBox newUser;
    newUser.user1 = *myUser;
    for(int j = 0; j<20; j++) {
	    newUser.mail[j]=emptyMessage();
    }
    mailboxes[i] = newUser;
    return USER_CREATE_SUCCESS;
}

/*QUIT function. removes the user created by START by removing identifiable information*/
int * quit_1_svc(user * myUser, struct svc_req * req){
    int userID = getIndexFromUser(myUser);
    users[userID].uuid = 0;
    for(int j = 0; j<20; j++) {
        mailboxes[userID].mail[j] = emptyMessage();
    }
    int * EXIT__SUCCESS = malloc(sizeof(int));
    int why = 1;
    EXIT__SUCCESS = &why;
    return EXIT__SUCCESS;
}

/*RETRIEVE_MESSAGE function. returns the char* of the message indicated in the parameters */
char ** retrieve_message_1_svc(struct retrieve_message_params * params, struct svc_req * req){
    user inQuestion = params->given_user;
    int messageNumber = params->message_number;
    printf("server : retrieving message %d from user %s%d", messageNumber, inQuestion.hostname, inQuestion.uuid);
    if(0<=messageNumber && messageNumber <20){
        int i = getIndexFromUser(&inQuestion);
        if(i<0) return NULL;//not found
        return &(mailboxes[i].mail[messageNumber].text);
    }
    return NULL;
}

/*LIST_ALL_MESSAGES returns all the messages in the given user's mailbox as a message block struct, NULL if not found */
message_block * list_all_messages_1_svc(user * myUser, struct svc_req * req){
    char* string = NULL;
    int userID = getIndexFromUser(myUser);
    if(userID<0) return NULL;
    for(int j = 0; j<20; j++) {
        string = strcat(string, mailboxes[userID].mail[j].text);
        string = strcat(string, "\n");
    }
    string = strcat(string, "\0");//dat null termination
    struct message_block * return_var = malloc(sizeof(struct message_block));
    struct message_block why = (struct message_block){.data=string};
    return_var = &why;
    return return_var;
}
/*DELETE_MESSAGE function. removes the message of the given index for the given user*/
int * delete_message_1_svc(delete_message_params * params, struct svc_req * req){
    int * MESSAGE_DELETE_SUCCESSFUL = malloc(sizeof(int));
    int why = 1;
    MESSAGE_DELETE_SUCCESSFUL = &why;
    why = -1;
    int * MESSAGE_DELETE_FAILED = malloc(sizeof(int));
    MESSAGE_DELETE_FAILED = &why;
    user givenUser = params->given_user;
    int messageNumber = params->message_number;
    int userID = getIndexFromUser(&givenUser);
    if(0<=messageNumber && messageNumber <20) {
        mailboxes[userID].mail[messageNumber] = emptyMessage();
        return MESSAGE_DELETE_SUCCESSFUL;
    }
    return MESSAGE_DELETE_FAILED;
}
/*INSERT_MESSAGE function. sets the message of the given index for the given user with the given string*/
int * insert_message_1_svc(insert_message_params * params, struct svc_req * req){
    int * MESSAGE_INSERT_SUCCESSFUL = malloc(sizeof(int));
    int why = 1;
    MESSAGE_INSERT_SUCCESSFUL = &why;
    why = -1;
    int * MESSAGE_INSERT_FAILED = malloc(sizeof(int));
    MESSAGE_INSERT_FAILED = &why;
    user givenUser = params->given_user;
    char* messageText = params->message;
    int messageNumber = params->message_number;
    int userID = getIndexFromUser(&givenUser);
    if(0<=messageNumber && messageNumber <20) {
        mailboxes[userID].mail[messageNumber] = (struct message){.text=messageText};
        return MESSAGE_INSERT_SUCCESSFUL;
    }
    return MESSAGE_INSERT_FAILED;
}
/*helper function, used to get the array index of a user in the array, since mailboxes are index-aligned*/
int getIndexFromUser(user * givenUser){
    int i;
    for(i = 0; i < sizeof(users) / sizeof(struct user); i++) {
        if(strcmp(users[i].hostname, givenUser->hostname) == 0 && users[i].uuid==givenUser->uuid){
            return i;
        }
    }
    return -1;
}
