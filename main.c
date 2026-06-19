#include "main.h"

unsigned int getTargetAddress(unsigned char arg1, unsigned char arg2) {
	unsigned int target_address = (arg1 << 8) | arg2;

	if (target_address % 4 != 0) {
		fprintf(stderr, "Error: Address from jump is not aligned for 4 bytes instructions!\n");
		exit(1);
	}

	return target_address;
}

int32_t switch_type(unsigned char type, unsigned char arg, unsigned char arg2, VM_DATA* vm_data) {
	switch (type) {
		case TYPE_IMM:
			return arg;
		case TYPE_REG:
			return vm_data->registers[arg];
		case TYPE_MEM: {
			unsigned int target_address = getTargetAddress(arg, arg2);
			return vm_data->memory[target_address];
		}
		default: 
			printf("ERROR: AT 0x%X invalid ADDRESSING TYPE!", *vm_data->p);
			exit(1);
	}
}

int main() {
	unsigned char instructions[69] = {
		MOV, 0x0, 0x0, 0x0,//ustaw r0 na 0
		CMP, 0x0, 0x0, 0xA, //porownaj R0 z 10
		JE, 0x0, 0x14,0x0, //jesli jest rowne to przejdz do konca
		ADD, 0x0, 0x0, 0x1, //dodaj 1 do r0
		JMP, 0x0, 0x4, 0x0, //skok do adresu 2
		END, 0x0, 0x0, 0x0 //END
	};

	int program_size = 0;

	for (int i = 0; i < 69; i += 4) {
		program_size += 4;
		if (instructions[i] == 0x00) { 
			break;
		}
	}

	unsigned int DATA_START = program_size;

	unsigned char* memory = calloc(1024 * 1024, sizeof(unsigned char));

	if (memory == NULL) {
		printf("ERROR: Memory allocation error! \n");
		return 1;
	}

	for (int i = 0; i < program_size; i++) {
		memory[i] = instructions[i];
	}

	int32_t registers[5] = { 0 };
	signed int flag = -2; // == 0 equal == 1 greater == -1 less
	int p = 0;

	VM_DATA vm_data;

	vm_data.memory = memory;
	vm_data.registers = registers;
	vm_data.flag = &flag;
	vm_data.p = &p;

	while (registers[STATUS] == 0) {
		if (p >= program_size) break;

		unsigned char opcode = memory[p];
		unsigned char arg1 = memory[p + 1];
		unsigned char arg2 = memory[p + 2];
		unsigned char arg3 = memory[p + 3];

		switch (opcode) {
		case MOV:
			registers[arg2] = switch_type(arg1, arg2, arg3, &vm_data);
			break;
		case ADD:
			registers[arg2] = registers[arg2] + switch_type(arg1, arg2, arg3, &vm_data);
			break;
		case SUB:
			registers[arg2] = registers[arg2] - switch_type(arg1, arg2, arg3, &vm_data);
			break;
		case MUL:
			registers[arg2] = registers[arg2] * switch_type(arg1, arg2, arg3, &vm_data);
			break;
		case XOR:
			registers[arg2] = registers[arg2] ^ switch_type(arg1, arg2, arg3, &vm_data);
			break;
		case CMP:
			if (registers[arg2] == switch_type(arg1, arg2, arg3, &vm_data)) {
				flag = 0;
			}
			else if (registers[arg2] > switch_type(arg1, arg2, arg3, &vm_data)) {
				flag = 1;
			}
			else {
				flag = -1;
			}
			break;
		case JE: {
			if (flag != 0) break;

			unsigned int target_address = getTargetAddress(arg1, arg2);

			p = target_address;
			continue;
		}
		case JMP: {
			unsigned int target_address = getTargetAddress(arg1, arg2);

			p = target_address;
			continue;
		}
		case JG: {
			if (flag != 1) break;

			unsigned int target_address = getTargetAddress(arg1, arg2);

			p = target_address;
			continue;
		}
		case JL: {
			if (flag != -1) break;

			unsigned int target_address = getTargetAddress(arg1, arg2);

			p = target_address;
			continue;
		}
		case DIV:
			registers[arg2] = registers[arg2] / switch_type(arg1, arg2, arg3, &vm_data);
			break;
		case JNE: {
			if (flag == 0) break;
			unsigned int target_address = getTargetAddress(arg1, arg2);

			p = target_address;

			continue;
		}
		case JLE: {
			if (flag == 1) break;
			unsigned int target_address = getTargetAddress(arg1, arg2);

			p = target_address;

			continue;
		}
		case JGE: {
			if (flag == -1) break;
			unsigned int target_address = getTargetAddress(arg1, arg2);

			p = target_address;

			continue;
		}
		default:
			registers[STATUS] = 1;
			break;
		}

		if (registers[R0] != 0) {
			printf("%d\n", registers[R0]);
		}

		p += 4;
	}

	free(memory);

	return 0;
}