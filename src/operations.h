//
// Created by golobii on 8/29/22.
//

#ifndef VM_OPERATIONS_H
#define VM_OPERATIONS_H

#include "types.h"

extern unsigned int ip;

void ins_load_const(struct vm *vm);
void ins_push(struct vm *vm);

void ins_add();
void ins_sub();

void ins_pop();
void ins_dup();
void ins_incr();

void ins_jmp(struct vm *vm);
void ins_jmpe(struct vm *vm);
void ins_jmpne(struct vm *vm);

#endif //VM_OPERATIONS_H
