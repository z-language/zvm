#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack/stack.h"
#include "parser/parser.h"
#include "types.h"

#define IOC(node) node->type == T_INT ? *((int*)node->data) : (char*)node->data
#define FROM_CONST_POOL() (žvalue *) vm.const_pool[vm.prog[++i]]
#define FREE(value) if (!value->isConst) free(value)

typedef enum {
    NOOP = 0x00,

    PUSH = 0x01,
    LOAD_CONST = 0x02,
    POP = 0x12,
    DUP = 0x13,

    ADD = 0x20,
    INCR,
    JMP = 0x30,
    JMPE = 0x31,
    JMPNE = 0x32,

    SYS = 0xfd,
    DEBUG = 0xfe,
    HLT = 0xff
} opcode;


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

int main() {
    struct vm vm;
    parse(&vm);

    /* printf("version: %d\n", vm.version);
    printf("size of const pool: %d\n", vm.size_of_const_pool);
    printf("size of program: %d\n", vm.size_of_prog);
    printf("%d\n", vm.const_pool[0]->integer);*/


    for (int i = 0; i < vm.size_of_prog; i++) {
        byte opcode = vm.prog[i];
        switch (opcode) {
            case LOAD_CONST:
                push(FROM_CONST_POOL());
                break;
            case PUSH: {
                žvalue *tmp = malloc(sizeof(žvalue));

                tmp->type = T_INT;
                tmp->integer = vm.prog[++i];
                tmp->isConst = false;
                push(tmp);
            }
                break;
            case ADD: {
                žvalue *b = pop();
                žvalue *a = pop();

                if (a->type != b->type) {
                    perror("ADD: Executed the add instruction on incompatible types.");
                    exit(EXIT_FAILURE);
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
                break;
            case POP: {
                žvalue *tmp = pop();
                puts("Pop\n");
                FREE(tmp);
            }
                break;
            case DUP: {
                žvalue *tmp = top();
                žvalue *new = malloc(sizeof(žvalue));
                memcpy(new, tmp, sizeof(žvalue));
                push(new);
            }
                break;
            case JMP: {
                žvalue *addr = FROM_CONST_POOL();

                if (addr->type != T_INT) {
                    perror("JMP: Provided address is not a number.");
                    exit(EXIT_FAILURE);
                }
                i = addr->integer - 1;
            }
                break;
            case JMPE: {
                žvalue *a = pop();
                žvalue *b = pop();
                žvalue *addr = FROM_CONST_POOL();

                if (!eq(a, b)) goto end;

                i = addr->integer - 1;
                end:
                FREE(a);
                FREE(b);
            }
                break;
            case JMPNE: {
                žvalue *a = pop();
                žvalue *b = pop();
                žvalue *addr = FROM_CONST_POOL();

                if (eq(a, b)) goto end;

                i = addr->integer - 1;
                FREE(a);
                FREE(b);
            }
                break;
            case DEBUG:
                display();
                break;
            case NOOP:
                break;
            case HLT:
                return 0;
        }
    }

    return 0;
}
