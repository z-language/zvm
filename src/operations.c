//
// Created by golobii on 8/29/22.
//

#include "operations.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack/stack.h"
#include "core/error.h"

#define FROM_CONST_POOL() (žvalue *) vm->const_pool[vm->prog[++ip]]
#define FREE(value) if (!value->isConst) free(value)

unsigned int ip = 0;

bool eq(žvalue *val1, žvalue *val2) {
    if (val1->type != val2->type) return false;

    switch (val1->type) {

        case T_INT:
            if (val1->integer == val2->integer) return true;
            break;
        case T_STR:
            if (strcmp(val1->string, val2->string) == 0) return true;
            break;
    }
    return false;
}


void ins_load_const(struct vm *vm) {
    push(FROM_CONST_POOL());
}

void ins_push(struct vm *vm) {
    žvalue *tmp = malloc(sizeof(žvalue));

    tmp->type = T_INT;
    tmp->integer = vm->prog[++ip];
    tmp->isConst = false;
    push(tmp);
}

void ins_add() {
    žvalue *b = pop();
    žvalue *a = pop();

    if (a->type != b->type) {
        throw("ADD: Executed the add instruction on incompatible types.");
    }
    žvalue *tmp = malloc(sizeof(žvalue));

    switch (a->type) {

        case T_STR:
            tmp->type = T_STR;

            unsigned int len = strlen(a->string) + strlen(b->string);
            char *new = malloc(sizeof(char) * len);
            strcat(new, a->string);
            strcat(new, b->string);
            tmp->string = new;
            push(tmp);
            break;
        case T_INT:
            tmp->type = T_INT;
            tmp->integer = a->integer + b->integer;
            push(tmp);
            break;
    }
    FREE(a);
    FREE(b);
}

void ins_sub() {
    žvalue *b = pop();
    žvalue *a = pop();

    if (a->type != b->type || a->type != T_INT) {
        throw("SUB: Tried to subtract incorrect types.");
    }
    žvalue *tmp = malloc(sizeof(žvalue));

    tmp->type = T_INT;
    tmp->integer = a->integer + b->integer;
    push(tmp);

    FREE(a);
    FREE(b);
}

void ins_pop() {
    žvalue *tmp = pop();
    puts("Pop\n");
    FREE(tmp);
}

void ins_dup() {
    žvalue *tmp = top();
    žvalue *new = malloc(sizeof(žvalue));
    memcpy(new, tmp, sizeof(žvalue));
    push(new);
}

void ins_jmp(struct vm *vm) {
    žvalue *addr = FROM_CONST_POOL();

    if (addr->type != T_INT) {
        throw("JMP: Provided address is not a number.");
    }
    ip = addr->integer - 1;
}

void ins_jmpe(struct vm *vm) {
    žvalue *a = pop();
    žvalue *b = pop();
    žvalue *addr = FROM_CONST_POOL();

    if (!eq(a, b)) goto end;

    ip = addr->integer - 1;
    end:
    FREE(a);
    FREE(b);
}

void ins_jmpne(struct vm *vm) {
    žvalue *a = pop();
    žvalue *b = pop();
    žvalue *addr = FROM_CONST_POOL();

    if (eq(a, b)) goto end;

    ip = addr->integer - 1;
    end:
    FREE(a);
    FREE(b);
}

void ins_incr() {
    top()->integer++;
}