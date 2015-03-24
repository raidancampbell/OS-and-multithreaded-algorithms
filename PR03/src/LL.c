//
// Created by Aidan Campbell on 3/24/15.
//

#include "LL.h"


#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct test_struct *head = NULL;
struct test_struct *curr = NULL;

struct test_struct* create_list(int val)
{
    printf("\n creating list with headnode as [%d]\n",val);
    struct test_struct *ptr = (struct test_struct*)malloc(sizeof(struct test_struct));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->next = NULL;

    head = curr = ptr;
    return ptr;
}

struct test_struct* add_to_list(struct test_struct *list,int val, bool add_to_end)
{
    if(NULL == list) return (create_list(val));

    if(add_to_end) printf("\n Adding node to end of list with value [%d]\n",val);
    else printf("\n Adding node to beginning of list with value [%d]\n",val);

    struct test_struct *ptr = (struct test_struct*)malloc(sizeof(struct test_struct));
    if(NULL == ptr) {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->next = NULL;
    //test_struct* end = getEnd(list);
    if(add_to_end == true) {
        getEnd(list)->next = ptr;
    } else {
        ptr->next = list;
        list = ptr;
    }
    return ptr;
}

struct test_struct* getEnd(struct test_struct *list){
    while(list->next != NULL)
    {
        list = list->next;
    }
    return list;
}

void deleteFirstRequest(struct test_struct *given){
    given = given->next;
}

int firstRequestAmount(struct test_struct *given){
    return given->val;
}

