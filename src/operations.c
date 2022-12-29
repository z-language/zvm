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

unsigned int call_stack[MAX_CALL_STACK_SIZE];
unsigned int *call_stack_ptr = call_stack;

unsigned int ip = 0;

#define CMP(val1, val2, op, output)               \
    if (val1->type != val2->type)           \
        output = false;                     \
    else {                                  \
        switch (val1->type) {              \
            case T_INT:                         \
                if (val1->integer op val2->integer) \
                    output = true;                        \
                break;                              \
            case T_STR:                         \
                if (strcmp(val1->string, val2->string) op 0)\
                    output = true;                        \
                break;                              \
        }                                           \
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
            break;
        case T_INT:
            tmp->type = T_INT;
            tmp->integer = a->integer + b->integer;
            break;
    }

    push(tmp);
    FREE(a);
    FREE(b);
}

void ins_mul() {
    žvalue *b = pop();
    žvalue *a = pop();

    if (a->type != b->type || a->type != T_INT) {
        throw("SUB: Tried to multiply incorrect types.");
    }
    žvalue *tmp = malloc(sizeof(žvalue));

    tmp->type = T_INT;
    tmp->integer = a->integer * b->integer;
    push(tmp);

    FREE(a);
    FREE(b);
}

void ins_div() {
    žvalue *b = pop();
    žvalue *a = pop();

    if (a->type != b->type || a->type != T_INT) {
        throw("DIV: Tried to divide incorrect types.");
    }
    žvalue *tmp = malloc(sizeof(žvalue));

    tmp->type = T_INT;
    tmp->integer = a->integer / b->integer;
    push(tmp);

    FREE(a);
    FREE(b);
}

void ins_mod() {
    žvalue *b = pop();
    žvalue *a = pop();

    if (a->type != b->type || a->type != T_INT) {
        throw("MOD: Tried to execute on incorrect types.");
    }
    žvalue *tmp = malloc(sizeof(žvalue));

    tmp->type = T_INT;
    tmp->integer = a->integer % b->integer;
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

void ins_jmpf() {
    žvalue *forward = pop();

    if (forward->type != T_INT) {
        throw("JMPF: Provided address is not a number.");
    }
    ip += forward->integer;

    FREE((forward));
}

void ins_jmpb() {
    žvalue *back = pop();

    if (back->type != T_INT) {
        throw("JMPB: Provided address is not a number.");
    }
    ip -= back->integer;

    FREE((back));
}

void ins_jmpt() {
    žvalue *forward = pop();
    žvalue *cond = pop();

    if (forward->type != T_INT) {
        throw("JMPT: Provided address is not a number.");
    }

    if (cond->integer != 0) {
        ip += forward->integer;
    }

    FREE(cond);
    FREE(forward);
}

void ins_call(struct vm *vm) {
    if ((call_stack_ptr - call_stack) + 1 == MAX_CALL_STACK_SIZE) {
        throw("Exceeded the call stack size!\n");
    }

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

void ins_eq() {
    žvalue *b = pop();
    žvalue *a = pop();

    žvalue *tmp = malloc(sizeof(žvalue));
    tmp->isConst = false;
    tmp->type = T_INT;

    bool equals = false;
    CMP(a, b, ==, equals);
    tmp->integer = equals;

    push(tmp);
    FREE(a);
    FREE(b);
}

void ins_and() {
    žvalue *b = pop();
    žvalue *a = pop();

    žvalue *tmp = malloc(sizeof(žvalue));
    tmp->isConst = false;
    tmp->type = T_INT;

    bool equals = false;
    CMP(a, b, &&, equals);
    tmp->integer = equals;

    push(tmp);
    FREE(a);
    FREE(b);
}
void ins_or() {
    žvalue *b = pop();
    žvalue *a = pop();

    žvalue *tmp = malloc(sizeof(žvalue));
    tmp->isConst = false;
    tmp->type = T_INT;

    bool equals = false;
    CMP(a, b, ||, equals);
    tmp->integer = equals;

    push(tmp);
    FREE(a);
    FREE(b);
}

void ins_greater_than() {
    žvalue *b = pop();
    žvalue *a = pop();

    žvalue *tmp = malloc(sizeof(žvalue));
    tmp->isConst = false;
    tmp->type = T_INT;

    bool equals = false;
    CMP(a, b, >, equals);
    tmp->integer = equals;

    push(tmp);
    FREE(a);
    FREE(b);
}

void ins_greater_or_eq() {
    žvalue *b = pop();
    žvalue *a = pop();

    žvalue *tmp = malloc(sizeof(žvalue));
    tmp->isConst = false;
    tmp->type = T_INT;

    bool equals = false;
    CMP(a, b, >=, equals);
    tmp->integer = equals;

    push(tmp);
    FREE(a);
    FREE(b);
}

void ins_less_than() {
    žvalue *b = pop();
    žvalue *a = pop();

    žvalue *tmp = malloc(sizeof(žvalue));
    tmp->isConst = false;
    tmp->type = T_INT;

    bool equals = false;
    CMP(a, b, <, equals);
    tmp->integer = equals;

    push(tmp);
    FREE(a);
    FREE(b);
}
void ins_less_or_eq() {
    žvalue *b = pop();
    žvalue *a = pop();

    žvalue *tmp = malloc(sizeof(žvalue));
    tmp->isConst = false;
    tmp->type = T_INT;

    bool equals = false;
    CMP(a, b, <=, equals);
    tmp->integer = equals;

    push(tmp);
    FREE(a);
    FREE(b);
}

void ins_incr() { top()->integer++; }
