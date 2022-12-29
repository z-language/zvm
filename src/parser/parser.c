//
// Created by golobii on 8/23/22.
//

#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long read_bytes(char *fileName, byte **buffer)
{
    FILE *fileptr;
    long filelen;

    fileptr = fopen(fileName, "rb");

    if (!fileptr)
    {
        return 0;
    }

    fseek(fileptr, 0, SEEK_END);
    filelen = ftell(fileptr);
    rewind(fileptr);

    *buffer = (byte *)malloc(filelen * sizeof(byte));
    fread(*buffer, filelen, 1, fileptr);
    fclose(fileptr);

    return filelen;
}

void parse(struct vm *vm, char *fileName)
{
    byte *buffer;
    long len = read_bytes(fileName, &buffer);
    if (!len)
    {
        char *format = "Couldn't open %s";
        char *out = malloc(sizeof(char) * (strlen(format) + strlen(fileName)));
        sprintf(out, format, fileName);
        perror(out);
        exit(EXIT_FAILURE);
    }
    unsigned int index = 0;

    vm->version = buffer[index++];
    vm->size_of_prog = (short)((buffer[index++] * 0x100) + buffer[index++]);
    vm->prog = malloc(sizeof(byte) * vm->size_of_prog);
    for (unsigned int i = 0; i < vm->size_of_prog; i++)
    {
        vm->prog[i] = buffer[index++];
    }

    vm->size_of_const_pool = (short)((buffer[index++] * 0x100) + buffer[index++]);
    vm->const_pool = malloc(sizeof(žvalue) * vm->size_of_const_pool);

    for (unsigned int i = 0; i < vm->size_of_const_pool; i++)
    {
        žvalue *new = malloc(sizeof(žvalue));
        new->type = buffer[index++];

        byte num_of_bytes = buffer[index++];
        byte *value = malloc(num_of_bytes);

        for (unsigned int j = 0; j < num_of_bytes; j++)
        {
            value[j] = buffer[index++];
        }

        if (new->type == T_INT)
        {
            new->integer = *(int *)value;
        }
        else if (new->type == T_STR)
        {
            new->string = (char *)value;
        }
        new->isConst = true;
        vm->const_pool[i] = new;
    }

    free(buffer);
}
