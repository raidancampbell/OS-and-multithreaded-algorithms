//
// Created by Aidan Campbell on 3/24/15.
//

#ifndef _ASN1_LL_H_
#define _ASN1_LL_H_
#include<stdbool.h>

struct node {
    int val;
    struct node *next;
};

struct node * create_list(int);
struct node * add_to_list(struct node *,int, bool);
struct node * deleteFirstRequest(struct node *);
struct node * getEnd(struct node *);
int firstRequestAmount(struct node *);

#endif //_ASN1_LL_H_
