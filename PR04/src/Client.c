//
// Created by Aidan Campbell on 4/10/15.
//
#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "MailBox.h"

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

    char hostname[81];
    hostname[80] = '\0';
    gethostname(hostname, 80);
    myUser = &(user) {.hostname=(string_wrapper){.data=hostname}, .uuid=42};

    /* Generate the client handle to call the server */
    if ((client=clnt_create(server,DISPLAY_PRG, DISPLAY_VER, "tcp")) == (CLIENT *) NULL) {
        clnt_pcreateerror(server);
        exit(2);
    }


    printf("client : Calling function.\n");
    return_value = start_1((void *) &myUser, client);

    if (*return_value)  printf("client : Mission accomplished.\n");
    else  printf("client : Unable to display message.\n");
    return 0;
}