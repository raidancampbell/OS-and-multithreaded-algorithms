//
// Created by Aidan Campbell on 4/10/15.
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

struct message emptyMessage(){
    char empty = ' ';
    return (struct message){.text=&empty};
}
//only run once.
//set each user's uuid to 0, and set the mailboxes to be full of empty messages
void init(){
    if(initFlag>0) return;
    initFlag++;

    int i;
    struct message emptyMail[20];
    for(int j = 0; j<20; j++) {
        emptyMail[j]=emptyMessage();
    }

    for(i = 0; i < sizeof(users) / sizeof(struct user); i++) {
        users[i].uuid = 0;
        mailboxes[i] = (struct userMailBox){.user1=NULL,.mail=emptyMail};
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

int * start_1_svc(user * myUser, struct svc_req * req) {
    init();
    //make a new mailBox for 'myUser'
    int USER_ALREADY_EXISTS = -1;
    int USER_CREATE_SUCCESS = 1;
    if(getIndexFromUser(myUser) >= 0) return &USER_ALREADY_EXISTS;//user already started
    int i = getFreeUser();

    users[i] = *myUser;
    struct message emptyMail[20];
    for(int j = 0; j<20; j++) {
        emptyMail[j]=emptyMessage();
    }
    mailboxes[i] = (struct userMailBox){.user1=*myUser,.mail=emptyMail};
    return &USER_CREATE_SUCCESS;
}

int * quit_1_svc(user * myUser, struct svc_req * req){
    int userID = getIndexFromUser(myUser);
    users[userID].uuid = 0;
    struct message emptyMail[20];
    for(int j = 0; j<20; j++) {
        emptyMail[j]=emptyMessage();
    }
    mailboxes[userID] = (struct userMailBox){.user1=*myUser,.mail=emptyMail};
}

char ** retrieve_message_1_svc(retrieve_message_params * params, struct svc_req * req){
    user inQuestion = retrieve_message_params.given_user;
    int messageNumber = retrieve_message_params.message_number;
    printf("server : retrieving message %d from user %s%d", messageNumber, inQuestion.hostname, inQuestion.uuid);
    if(0<=messageNumber && messageNumber <20){
        int i = getIndexFromUser(&inQuestion);
        if(i<0) return NULL;//not found
        return &(mailboxes[i].mail[messageNumber].text);
    }
    return NULL;
}

message_block * list_all_messages_1_svc(user * myUser, struct svc_req * req){
    char* string = NULL;
    int userID = getIndexFromUser(myUser);
    for(int j = 0; j<20; j++) {
        string = strcat(string, mailboxes[userID].mail[j]);
        string = strcat(string, '\n');
    }
    return (message_block*)&{.data=string};
}

int * delete_message_1_svc(delete_message_params * params, struct svc_req * req){
    int MESSAGE_DELETE_SUCCESSFUL = 1;
    int MESSAGE_DELETE_FAILED = -1;
    user givenUser = params->given_user;
    int messageNumber = params->message_number;
    int userID = getIndexFromUser(&givenUser);
    if(0<=messageNumber && messageNumber <20) {
        mailboxes[userID].mail[messageNumber] = emptyMessage();
        return &MESSAGE_DELETE_SUCCESSFUL;
    }
    return &MESSAGE_DELETE_FAILED;
}

int * insert_message_1_svc(insert_message_params * params, struct svc_req * req){
    int MESSAGE_DELETE_SUCCESSFUL = 1;
    int MESSAGE_DELETE_FAILED = -1;
    user givenUser = params->given_user;
    char* messageText = params->message;
    int messageNumber = params->message_number;
    int userID = getIndexFromUser(&givenUser);
    if(0<=messageNumber && messageNumber <20) {
        mailboxes[userID].mail[messageNumber] = (struct message){.text=messageText};
        return &MESSAGE_DELETE_SUCCESSFUL;
    }
    return &MESSAGE_DELETE_FAILED;
}

int getIndexFromUser(user * givenUser){
    int i;
    for(i = 0; i < sizeof(users) / sizeof(struct user); i++)
    {
        if(strcmp(users[i].hostname, givenUser->hostname) == 0 && users[i].uuid==givenUser->uuid){
            return i;
        }
    }
    return -1;
}