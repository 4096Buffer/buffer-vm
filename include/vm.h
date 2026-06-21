#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MEMORY_SIZE (1024 * 1024)
#define STACK_SIZE  (64 * 1024)

#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define STATUS 4

typedef struct {
    int32_t* registers;
    signed int* flag;
    unsigned char* memory;
    int* p;
    int* sp;
} VM_DATA;

unsigned int getTargetAddress(unsigned char arg1, unsigned char arg2);
int32_t switchType(unsigned char type, unsigned char arg, unsigned char arg2, VM_DATA* vm_data);
void writeU32(unsigned char* memory, unsigned int addr, int32_t value);
int32_t readU32(unsigned char* memory, unsigned int addr);

void vmExecute(VM_DATA* vm_data, int program_size);