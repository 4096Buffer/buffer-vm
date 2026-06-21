#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define R0 0
#define R1 1
#define R2 2 
#define R3 3
#define STATUS 4

#define MOV 0x1
#define CMP 0x6  
#define JE  0x7
#define ADD 0x2
#define SUB 0x3
#define MUL 0x4
#define XOR 0x5
#define JMP 0x8
#define END 0x0
#define JG  0x9
#define JL  0xA
#define DIV 0xB
#define JNE 0xC
#define JLE 0xD
#define JGE 0xE

#define PUSH 0xF
#define POP 0x10
#define CALL 0x11
#define RET 0x12

#define LOAD   0x13   
#define STORE  0x14

#define PRINT 0x15

#define TYPE_IMM 0x0
#define TYPE_REG 0x1
#define TYPE_MEM 0x2

typedef struct {
	int32_t* registers;
	signed int* flag;
	unsigned char* memory;
	int* p;
} VM_DATA;

unsigned int getTargetAddress(unsigned char arg1, unsigned char arg2);
int32_t switchType(unsigned char type, unsigned char arg, unsigned char arg2, VM_DATA* vm_data);