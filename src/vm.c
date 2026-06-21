#include "vm.h"
#include "opcodes.h"

void vmExecute(VM_DATA* vm_data, int program_size) {
	int* p = vm_data->p;
	unsigned char* memory = vm_data->memory;
	int32_t* registers = vm_data->registers;
	int* flag = vm_data->flag;
	int* sp = vm_data->sp;

	while (vm_data->registers[STATUS] == 0) {
		if (*p >= program_size) break;

		unsigned char opcode = memory[*p];
		unsigned char arg1 = memory[*p + 1];
		unsigned char arg2 = memory[*p + 2];
		unsigned char arg3 = memory[*p + 3];

		switch (opcode) {
		case MOV:
			registers[arg2] = switchType(arg1, arg2, arg3, vm_data);
			break;
		case ADD:
			registers[arg2] = registers[arg2] + switchType(arg1, arg2, arg3, vm_data);
			break;
		case SUB:
			registers[arg2] = registers[arg2] - switchType(arg1, arg2, arg3, vm_data);
			break;
		case MUL:
			registers[arg2] = registers[arg2] * switchType(arg1, arg2, arg3, vm_data);
			break;
		case XOR:
			registers[arg2] = registers[arg2] ^ switchType(arg1, arg2, arg3, vm_data);
			break;
		case CMP: {
			int32_t switch_chose = switchType(arg1, arg2, arg3, vm_data);

			if (registers[arg2] == switch_chose) {
				*flag = 0;
			}
			else if (registers[arg2] > switch_chose) {
				*flag = 1;
			}
			else {
				*flag = -1;
			}
			break;
		}
		case JE: {
			if (*flag != 0) break;

			unsigned int target_address = getTargetAddress(arg2, arg3);

			*p = target_address;
			continue;
		}
		case JMP: {
			unsigned int target_address = getTargetAddress(arg2, arg3);

			*p = target_address;
			continue;
		}
		case JG: {
			if (*flag != 1) break;

			unsigned int target_address = getTargetAddress(arg2, arg3);

			*p = target_address;
			continue;
		}
		case JL: {
			if (*flag != -1) break;

			unsigned int target_address = getTargetAddress(arg2, arg3);

			*p = target_address;
			continue;
		}
		case DIV:
			registers[arg2] = registers[arg2] / switchType(arg1, arg2, arg3, vm_data);
			break;
		case JNE: {
			if (*flag == 0) break;
			unsigned int target_address = getTargetAddress(arg2, arg3);

			*p = target_address;

			continue;
		}
		case JLE: {
			if (*flag == 1) break;
			unsigned int target_address = getTargetAddress(arg2, arg3);

			*p = target_address;

			continue;
		}
		case JGE: {
			if (*flag == -1) break;
			unsigned int target_address = getTargetAddress(arg2, arg3);

			*p = target_address;

			continue;
		}
		case LOAD: {
			unsigned int target_address = getTargetAddress(arg2, arg3);

			// here arg1 represents the index of register not type

			registers[arg1] = *(memory + target_address);

			break;
		}
		case STORE: {
			unsigned int target_address = getTargetAddress(arg2, arg3);

			*(memory + target_address) = registers[arg1];
			break;
		}
		case PRINT: {
			if (arg1 == TYPE_MEM) {
				unsigned int target_address = getTargetAddress(arg2, arg3);

				while (memory[target_address] != '\0') {
					putchar(memory[target_address]);
					target_address++;
				}
			}
			else {
				int32_t value = switchType(arg1, arg2, arg3, vm_data);

				printf("%d", value);
			}
			break;
		}
		case PUSH: {
			*sp -= 4;
			writeU32(memory, *sp, registers[arg1]);
			break;
		}
		case POP: {
			registers[arg1] = readU32(memory, *sp);
			*sp += 4;
			break;
		}
		case CALL: {
			unsigned int target_address = getTargetAddress(arg2, arg3);

			*sp -= 4;
			writeU32(memory, *sp, *p + 4);

			*p = target_address;
			continue;
		}
		case RET: {
			int return_address = readU32(memory, *sp);
			*sp += 4;

			*p = return_address;
			continue;
		}
		case AND:
			registers[arg2] = registers[arg2] & switchType(arg1, arg2, arg3, vm_data);
			break;
		case OR:
			registers[arg2] = registers[arg2] | switchType(arg1, arg2, arg3, vm_data);
			break;
		case NOT:
			registers[arg2] = ~registers[arg2];
			break;
		case SHL:
			registers[arg2] = registers[arg2] << switchType(arg1, arg2, arg3, vm_data);
			break;
		case SHR:
			registers[arg2] = registers[arg2] >> switchType(arg1, arg2, arg3, vm_data);
			break;
		default:
			registers[STATUS] = 1;
			break;
		}

		*p += 4;
	}

}