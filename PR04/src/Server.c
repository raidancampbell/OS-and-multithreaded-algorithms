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

struct user users[1];
struct userMailBox mailboxes[1];

int * start_1_svc(user * myUser, struct svc_req * req) {
    static int  ok;
    ok = printf("server : Hello, world.\n");
    return (&ok);
}

int * quit_1_svc(user * myUser, struct svc_req * req){

}

char ** retrieve_message_1_svc(retrieve_message_params *, struct svc_req *){
    user inQuestion = retrieve_message_params.given_user;
    int messageNumber = retrieve_message_params.message_number;
    printf("server : retrieving message %d from user %s%d", messageNumber, inQuestion.hostname, inQuestion.uuid);
    if(0<=messageNumber && messageNumber <20){
        int i;
        for(i = 0; i < sizeof(users) / sizeof(struct user); i++)
        {
           if(strcmp(users[i].hostname, inQuestion.hostname) == 0 && users[i].uuid==inQuestion.uuid){
               return &(mailboxes[i].mail[messageNumber].text);
           }
        }
    }
    return NULL;
}

message_block * list_all_messages_1_svc(user *, struct svc_req *){

}

int * delete_message_1_svc(delete_message_params *, struct svc_req *){

}

int * insert_message_1_svc(insert_message_params *, struct svc_req *){

}