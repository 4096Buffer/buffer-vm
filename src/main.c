#include "opcodes.h"
#include "vm.h"

int main() {
	unsigned char instructions[69] = {
		MOV,   TYPE_IMM, 0x0, 0x0,

		CMP,   TYPE_IMM, 0x0, 0x5,

		JE,    0x0, 0x0, 0x20,

		PRINT, TYPE_MEM, 0x0, 0x24, 

		PRINT, TYPE_REG, 0x0, 0x0, 

		PRINT, TYPE_MEM, 0x0, 0x2C,

		ADD,   TYPE_IMM, 0x0, 0x1,

		JMP,   0x0, 0x0, 0x4,

		END,   0x0, 0x0, 0x0,

		'I', 't', 'e', 'r', ':', ' ', '\0', 0x0,

		'\n', '\0'
	};

	int program_size = 0;

	for (int i = 0; i < 69; i += 4) {
		program_size += 4;
		if (instructions[i] == 0x00) { 
			break;
		}
	}

	unsigned int DATA_START = program_size;
	unsigned char* memory = calloc(MEMORY_SIZE, sizeof(unsigned char));

	if (memory == NULL) {
		printf("ERROR: Memory allocation error! \n");
		return 1;
	}

	for (int i = 0; i < 69; i++) {  
		memory[i] = instructions[i];
	}

	int32_t registers[5] = { 0 };
	signed int flag = -2;
	int p = 0;
	int sp = MEMORY_SIZE;

	VM_DATA vm_data;

	vm_data.memory = memory;
	vm_data.registers = registers;
	vm_data.flag = &flag;
	vm_data.p = &p;
	vm_data.sp = &sp;

	vmExecute(&vm_data, program_size);

	free(memory);

	return 0;
}