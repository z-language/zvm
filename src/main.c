#include <stdio.h>
#include <stdlib.h>

#include "core/error.h"
#include "operations.h"
#include "parser/parser.h"
#include "stack/stack.h"
#include "types.h"

typedef enum {
    NOOP = 0x00,

    PUSH = 0x01,
    LOAD_CONST = 0x02,
    STORE_NAME = 0x03,
    LOAD_NAME = 0x04,
    POP = 0x12,
    DUP = 0x13,

    ADD = 0x20,
    SUB = 0x21,
    MUL = 0x22,
    DIV = 0x23,
    MOD = 0x24,
    EQ = 0x25,
    GREATER_THAN = 0x26,
    GREATER_OR_EQ = 0x27,
    LESS_THAN = 0x28,
    LESS_OR_EQ = 0x29,
    AND = 0x30,
    OR = 0x31,

    JMP = 0x40,
    JMPF = 0x41,
    JMPB = 0x42,
    JMPT = 0x43,

    CALL = 0x50,
    RETURN = 0x51,

    SYS = 0xfd,
    DEBUG = 0xfe,
    HLT = 0xff
} opcode;

int main(int argc, char **argv) {
    if (argc <= 1)
        throw("Not enough arguments supplied.\n");

    struct vm vm;
    parse(&vm, argv[1]);

    /* printf("version: %d\n", vm.version);
    printf("size of const pool: %d\n", vm.size_of_const_pool);
    printf("size of program: %d\n", vm.size_of_prog);
    printf("%d\n", vm.const_pool[0]->integer);*/

    for (; ip < vm.size_of_prog; ip++) {
        byte opcode = vm.prog[ip];
        switch (opcode) {
            case LOAD_CONST:
                ins_load_const(&vm);
                break;
            case STORE_NAME:
                ins_store_name(&vm);
                break;
            case LOAD_NAME:
                ins_load_name(&vm);
                break;
            case PUSH:
                ins_push(&vm);
                break;
            case ADD:
                ins_add();
                break;
            case SUB:
                ins_sub();
                break;
            case MUL:
                ins_mul();
                break;
            case DIV:
                ins_div();
                break;
            case MOD:
                ins_mod();
                break;
            case EQ:
                ins_eq();
                break;
            case GREATER_THAN:
                ins_greater_than();
                break;
            case GREATER_OR_EQ:
                ins_greater_or_eq();
                break;
            case LESS_THAN:
                ins_less_than();
                break;
            case LESS_OR_EQ:
                ins_less_or_eq();
                break;
            case AND:
                ins_and();
                break;
            case OR:
                ins_or();
                break;
            case POP:
                ins_pop();
                break;
            case DUP:
                ins_dup();
                break;
            case JMP:
                ins_jmp(&vm);
                break;
            case JMPF:
                ins_jmpf();
                break;
            case JMPB:
                ins_jmpb();
                break;
            case JMPT:
                ins_jmpt();
                break;
            case CALL:
                ins_call(&vm);
                break;
            case RETURN:
                ins_return();
                break;
            case SYS:
                ins_sys();
                break;
            case DEBUG:
                display();
                break;
            case NOOP:
                break;
            case HLT:
                return 0;
            default:
                throw("Tried to execute an unknown instruction.");
        }
    }

    free(&vm);

    return 0;
}
