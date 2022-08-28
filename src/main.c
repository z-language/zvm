#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack/stack.h"
#include "parser/parser.h"
#include "types.h"

#define IOC(node) node->type == T_INT ? *((int*)node->data) : (char*)node->data
#define FROM_CONST_POOL() (žvalue *) vm.const_pool[vm.prog[++i]]

typedef enum {
    NOOP = 0x00,

    PUSH = 0x01,
    LOAD_CONST = 0x02,
    POP = 0x12,
    DUP,

    ADD = 0x20,
    INCR,
    JMP = 0x30,
    JMPE = 0x31,
    JMPNE = 0x32,

    SYS = 0xfd,
    DEBUG = 0xfe,
    HLT = 0xff
} opcode;



int main() {
    struct vm vm;
    parse(&vm);

    /* printf("version: %d\n", vm.version);
    printf("size of const pool: %d\n", vm.size_of_const_pool);
    printf("size of program: %d\n", vm.size_of_prog);
    printf("%d\n", vm.const_pool[0]->integer);*/


    for (int i = 0; i < vm.size_of_prog; i++) {
        switch (vm.prog[i]) {
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
                free(a);
                free(b);
            }
                break;
            case POP: {
                žvalue *tmp = pop();
                puts("Pop\n");
                if (tmp->isConst) continue;
                else free(tmp);
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

                if (memcmp(a, b, sizeof(žvalue)) != 0) goto end;

                i = addr->integer - 1;
                end:
                free(a);
                free(b);
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
