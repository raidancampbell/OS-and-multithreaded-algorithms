//
// Created by Aidan Campbell on 4/10/15.
//
#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MailBox.h"


/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };


void * start_1(user *givenUser, CLIENT *clnt)
{
    static int clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));

    if (clnt_call (clnt, start,
                   (xdrproc_t) xdr_void, (caddr_t) givenUser,
                   (xdrproc_t) xdr_int, (caddr_t) &clnt_res,
                   TIMEOUT) != RPC_SUCCESS) {
        return &clnt_res;
    }

}

void * quit_1(user *givenUser, CLIENT *clnt)
{
    static int clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));

    if (clnt_call (clnt, quit,
                   (xdrproc_t) xdr_void, (caddr_t) givenUser,
                   (xdrproc_t) xdr_int, (caddr_t) &clnt_res,
                   TIMEOUT) != RPC_SUCCESS) {
        return &clnt_res;
    }

}

string_wrapper * retrieve_message_1(retrieve_message_params *givenParams, CLIENT *clnt){
    static int clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));

    if (clnt_call (clnt, retrieve_message,
                   (xdrproc_t) xdr_void, (caddr_t) givenParams,
                   (xdrproc_t) xdr_int, (caddr_t) &clnt_res,
                   TIMEOUT) != RPC_SUCCESS) {
        return &clnt_res;
    }

}

string_wrapper * list_all_messages_1(user *givenUser, CLIENT *clnt){
    static int clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));

    if (clnt_call (clnt, list_all_messages,
                   (xdrproc_t) xdr_void, (caddr_t) givenUser,
                   (xdrproc_t) xdr_int, (caddr_t) &clnt_res,
                   TIMEOUT) != RPC_SUCCESS) {
        return &clnt_res;
    }

}

void * delete_message_1(delete_message_params *givenParams, CLIENT *clnt){
    static int clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));

    if (clnt_call (clnt, delete_message,
                   (xdrproc_t) xdr_void, (caddr_t) givenParams,
                   (xdrproc_t) xdr_int, (caddr_t) &clnt_res,
                   TIMEOUT) != RPC_SUCCESS) {
        return &clnt_res;
    }

}