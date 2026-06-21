#include "vm.h"
#include "opcodes.h"

unsigned int getTargetAddress(unsigned char arg1, unsigned char arg2) {
	unsigned int target_address = (arg1 << 8) | arg2;

	if (target_address % 4 != 0) {
		fprintf(stderr, "Error: Address from jump is not aligned for 4 bytes instructions!\n");
		exit(1);
	}

	return target_address;
}

int32_t switchType(unsigned char type, unsigned char arg, unsigned char arg2, VM_DATA* vm_data) {
	switch (type) {
	case TYPE_IMM:
		return arg2;
	case TYPE_REG:
		return vm_data->registers[arg2];
	case TYPE_MEM: {
		unsigned int target_address = getTargetAddress(arg, arg2);
		return vm_data->memory[target_address];
	}
	default:
		printf("ERROR: AT 0x%X invalid ADDRESSING TYPE!", *vm_data->p);
		exit(1);
	}
}

void writeU32(unsigned char* memory, unsigned int addr, int32_t value) {
	memory[addr] = (value >> 24) & 0xFF;
	memory[addr + 1] = (value >> 16) & 0xFF;
	memory[addr + 2] = (value >> 8) & 0xFF;
	memory[addr + 3] = value & 0xFF;
}

int32_t readU32(unsigned char* memory, unsigned int addr) {
	return (memory[addr] << 24) | (memory[addr + 1] << 16) | (memory[addr + 2] << 8) | memory[addr + 3];
}