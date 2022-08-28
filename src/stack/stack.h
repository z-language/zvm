//
// Created by golobii on 8/21/22.
//

#ifndef VM_STACK_H
#define VM_STACK_H

#include "../types.h"

#define STACK_SIZE 300

extern unsigned int sp;

void display();
void push(žvalue *data);
žvalue *pop();
žvalue *top();


#endif //VM_STACK_H
