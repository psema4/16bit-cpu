// alu.c: The calculator on the desk

#include "cpu.h"
#include "opcodes.h"
#include <stdio.h>

// The calculator automatically updates the status board after every operation
void update_flags(CPU *cpu, uint16_t result, uint16_t a, uint16_t b, Opcode op) {
    cpu->flags = 0;  // All lights off first

    if (result == 0)     cpu->flags |= FLAG_ZERO;      // Zero light
    if (result & 0x8000) cpu->flags |= FLAG_NEGATIVE;   // Negative light

    // Overflow: both inputs had the same sign, but result has a different sign
    if (op == OP_ADD) {
        if ((a ^ result) & (b ^ result) & 0x8000)
            cpu->flags |= FLAG_OVERFLOW;
    } else if (op == OP_SUB || op == OP_CMP) {
        if ((a ^ b) & (a ^ result) & 0x8000)
            cpu->flags |= FLAG_OVERFLOW;
    }
}

// Press a button on the calculator
uint16_t alu_execute(CPU *cpu, Opcode op, uint16_t a, uint16_t b) {
    uint16_t result = 0;

    switch (op) {
        case OP_ADD: result = a + b;           break;
        case OP_SUB:
        case OP_CMP: result = a - b;           break;
        case OP_AND: result = a & b;           break;
        case OP_OR:  result = a | b;           break;
        case OP_XOR: result = a ^ b;           break;
        case OP_NOT: result = ~a;              break;
        case OP_SHL: result = a << (b & 0xF);  break;
        case OP_SHR: result = a >> (b & 0xF);  break;
        default:
            fprintf(stderr, "Calculator error: unknown operation 0x%X\n", op);
            return 0;
    }

    update_flags(cpu, result, a, b, op);
    return result;
}