//
// Created by Aidan Campbell on 4/10/15.
//

#include <rpc/rpc.h>
#include <stdlib.h>
#include <stdio.h>
#include "MailBox.h"

int * print_hello_1_svc(void * filler, struct svc_req * req) {
    static int  ok;
    ok = printf("server : Hello, world.\n");
    return (&ok);
}