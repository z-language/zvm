//
// Created by golobii on 8/29/22.
//

#include "operations.h"

#include "core/error.h"
#include "stack/stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FROM_CONST_POOL() (žvalue *)vm->const_pool[vm->prog[++ip]]
#define FREE(value)    \
  if (!value->isConst) \
  free(value)
#define VALUE(e) (e->type == T_INT ? e->integer : e->string)

#define MAX_NAMES 255

#define MAX_CALL_STACK_SIZE 200

žvalue *names[MAX_NAMES];

unsigned int call_stack[200];
unsigned int *call_stack_ptr = call_stack;

unsigned int ip = 0;

bool eq(žvalue *val1, žvalue *val2) {
    if (val1->type != val2->type)
        return false;

    switch (val1->type) {

        case T_INT:
            if (val1->integer == val2->integer)
                return true;
            break;
        case T_STR:
            if (strcmp(val1->string, val2->string) == 0)
                return true;
            break;
    }
    return false;
}

void ins_load_const(struct vm *vm) { push(FROM_CONST_POOL()); }

void ins_load_name(struct vm *vm) {
    byte index = vm->prog[++ip];
    žvalue *tmp = malloc(sizeof(žvalue));
    memcpy(tmp, names[index], sizeof(žvalue));

    push(tmp);
}

void ins_store_name(struct vm *vm) {
    žvalue *tmp = pop();
    byte index = vm->prog[++ip];

    names[index] = tmp;
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

void ins_mul() {
    žvalue *b = pop();
    žvalue *a = pop();

    if (a->type != b->type || a->type != T_INT) {
        throw("SUB: Tried to subtract incorrect types.");
    }
    žvalue *tmp = malloc(sizeof(žvalue));

    tmp->type = T_INT;
    tmp->integer = a->integer * b->integer;
    push(tmp);

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
    tmp->integer = a->integer - b->integer;
    push(tmp);

    FREE(a);
    FREE(b);
}

void ins_pop() {
    žvalue *tmp = pop();
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

    if (!eq(a, b))
        goto end;

    ip = addr->integer - 1;
    end:
    FREE(a);
    FREE(b);
}

void ins_call(struct vm *vm) {
    *(++call_stack_ptr) = ip + 1;
    žvalue *addr = FROM_CONST_POOL();

    if (addr->type != T_INT) {
        throw("CALL: Provided address is not a number.");
    }

    ip = addr->integer - 1;
}

void ins_return() {
    ip = *(call_stack_ptr--);
}

void ins_sys() {
    žvalue *d = pop();
    žvalue *c = pop();
    žvalue *b = pop();
    žvalue *a = pop();

    syscall((long) a, VALUE(b), VALUE(c), VALUE(d));

    FREE(a);
    FREE(b);
    FREE(c);
    FREE(d);
}

void ins_jmpne(struct vm *vm) {
    žvalue *a = pop();
    žvalue *b = pop();
    žvalue *addr = FROM_CONST_POOL();

    if (eq(a, b))
        goto end;

    ip = addr->integer - 1;
    end:
    FREE(a);
    FREE(b);
}

void ins_incr() { top()->integer++; }
