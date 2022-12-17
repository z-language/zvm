//
// Created by golobii on 8/21/22.
//

#ifndef VM_TYPES_H
#define VM_TYPES_H

#include <stdbool.h>

typedef unsigned char byte;

enum Type
{
  T_STR = 0x01,
  T_INT = 0x02,
};

typedef struct
{
  enum Type type;
  bool isConst;
  union
  {
    int integer;
    char *string;
  };

} žvalue;

struct vm
{
  byte version; // bytecode format version

  short size_of_const_pool; // number of constants in constant pool
  žvalue **const_pool;

  short size_of_prog;
  byte *prog; // an array of bytes/instructions that will be loaded into memory
              // and executed one by one
};

#endif // VM_TYPES_H
