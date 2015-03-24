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

    if(add_to_end) {
        getEnd(list)->next = ptr;
        getEnd(list) = ptr;
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

void DeleteFirstRequest(struct test_struct *given){
    given = given->next;
}

int main(void) {
    int i = 0, ret = 0;
    struct test_struct *ptr = NULL;

    print_list();

    for(i = 5; i<10; i++)
        add_to_list(i,true);

    print_list();

    for(i = 4; i>0; i--)
        add_to_list(i,false);

    print_list();

    for(i = 1; i<10; i += 4)
    {
        ptr = search_in_list(i, NULL);
        if(NULL == ptr) {
            printf("\n Search [val = %d] failed, no such element found\n",i);
        }
        else
        {
            printf("\n Search passed [val = %d]\n",ptr->val);
        }

        print_list();

        ret = delete_from_list(i);
        if(ret != 0)
        {
            printf("\n delete [val = %d] failed, no such element found\n",i);
        }
        else
        {
            printf("\n delete [val = %d]  passed \n",i);
        }

        print_list();
    }

    return 0;
}