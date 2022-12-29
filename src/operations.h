//
// Created by golobii on 8/29/22.
//

#ifndef VM_OPERATIONS_H

#define VM_OPERATIONS_H

#include "types.h"

extern unsigned int ip;

void ins_load_const(struct vm *vm);
void ins_load_name(struct vm *vm);
void ins_store_name(struct vm *vm);
void ins_push(struct vm *vm);

void ins_add();
void ins_sub();
void ins_mul();
void ins_div();
void ins_mod();
void ins_eq();
void ins_greater_than();
void ins_greater_or_eq();
void ins_less_than();
void ins_less_or_eq();
void ins_and();
void ins_or();

void ins_pop();
void ins_dup();
void ins_incr();

void ins_call(struct vm *vm);
void ins_return();

void ins_sys();
void ins_jmp(struct vm *vm);
void ins_jmpf();
void ins_jmpb();
void ins_jmpt();

#endif // VM_OPERATIONS_H
