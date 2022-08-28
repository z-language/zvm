//
// Created by golobii on 8/21/22.
//

#include "stack.h"

#include <stdio.h>

žvalue *stack[STACK_SIZE] = {0};
unsigned int sp = 0;

void push(žvalue *data)
{
    stack[sp++] = data;
}

void display()
{
    žvalue *current;
    for (unsigned int i = 0; i < sp; i++) {
        current = stack[i];
        switch (current->type) {

            case T_STR:
                printf("%s\n", current->string);
                break;
            case T_INT:
                printf("%d\n", current->integer);
                break;
        }
    }
}

žvalue *pop()
{
    return stack[--sp];
}

žvalue *top()
{
    return stack[sp - 1];
}