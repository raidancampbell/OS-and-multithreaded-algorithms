//
// Created by Aidan Campbell on 3/24/15.
//

#ifndef _ASN1_LL_H_
#define _ASN1_LL_H_
#include<stdbool.h>

struct test_struct {
    int val;
    struct test_struct *next;
};

struct test_struct* create_list(int);
struct test_struct* add_to_list(struct test_struct *,int, bool);
void deleteFirstRequest(struct test_struct*);
struct test_struct* getEnd(struct test_struct*);
int firstRequestAmount(struct test_struct*);

#endif //_ASN1_LL_H_
