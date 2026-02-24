// memory.c: The filing cabinet operations

#include <string.h>
#include <stdio.h>
#include "cpu.h"

// Opening day: reset the entire office
void cpu_init(CPU *cpu) {
    memset(cpu->registers, 0, sizeof(cpu->registers));
    memset(cpu->memory, 0, sizeof(cpu->memory));
    cpu->pc = 0x0000;
    cpu->sp = 0xFFFF;
    cpu->flags = 0;
    cpu->halted = false;
    cpu->cycles = 0;
}

// Walk to the cabinet, grab two folders, combine them into a 16-bit value
uint16_t mem_read16(CPU *cpu, uint16_t address) {
    uint16_t low  = cpu->memory[address];
    uint16_t high = cpu->memory[address + 1];
    return (high << 8) | low;
}

// Walk to the cabinet, split a 16-bit value across two folders
void mem_write16(CPU *cpu, uint16_t address, uint16_t value) {
    cpu->memory[address]     = value & 0xFF;
    cpu->memory[address + 1] = (value >> 8) & 0xFF;
}