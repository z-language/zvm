# ŽVM INSTRUCTION SET AND DOCUMENTATION


The file format in a c-like representation.
```c
typedef unsigned char byte;

struct constant {
    byte type; // type of constant (see types enum) TODO
    byte num_of_bytes; // number of bytes this constant occupies    
    byte value[num_of_bytes]; // the value of the constant
};

struct source {
    byte version; // bytecode format version
    short size_of_prog;
    byte prog[size_of_prog]; // an array of bytes/instructions that will be loaded into memory and executed one by one
    short size_of_const_pool; // number of constants in constant pool
    struct constant const_pool[size_of_const_pool];
};
```