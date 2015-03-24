//
// Created by Aidan Campbell on 3/24/15.
// courtesy of, and adapted from: http://www.thegeekstuff.com/2012/08/c-linked-list-example/

#include "LL.h"


#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct node * create_list(int val) {
    printf("\ncreating list with headnode as [%d]",val);
    struct node *ptr = (struct node *)malloc(sizeof(struct node));
    if(NULL == ptr) {
        printf("\nNode creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->next = NULL;
    printf("\nlist creation successful");
    return ptr;
}

struct node * add_to_list(struct node *list,int val, bool add_to_end)
{
    if(NULL == list) return (create_list(val));

    if(add_to_end) printf("\n Adding node to end of list with value [%d]\n",val);
    else printf("\n Adding node to beginning of list with value [%d]\n",val);

    struct node *ptr = (struct node *)malloc(sizeof(struct node));
    if(NULL == ptr) {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->next = NULL;
    if(add_to_end == true) {
        printf("\nAdding to end of list");
        getEnd(list)->next = ptr;
    } else {
        printf("\nAdding to beginning of list");
        ptr->next = list;
        list = ptr;
    }
    return ptr;
}

struct node * getEnd(struct node *list) {
    printf("Getting end of list.");
    if(list==NULL) return list;
    struct node * temp = list;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    return temp;
}

struct node * deleteFirstRequest(struct node *given){
    if(given == NULL) {
        printf("\nERROR: DELETE_FIRST_REQUEST CALLED ON NULL NODE");
        return given;
    }
    return given->next;
}

int firstRequestAmount(struct node *given){
    if(given == NULL) {
        printf("\nERROR: FIRST_REQUEST_AMOUNT CALLED ON NULL NODE");
        return -1;
    }
    return given->val;
}
