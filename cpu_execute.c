// cpu_execute.c: The worker's daily routine

#include <stdio.h>
#include <inttypes.h>
#include "cpu.h"
#include "opcodes.h"
#include "instruction.h"

extern uint16_t alu_execute(CPU *cpu, Opcode op, uint16_t a, uint16_t b);
extern uint16_t mem_read16(CPU *cpu, uint16_t address);
extern void mem_write16(CPU *cpu, uint16_t address, uint16_t value);

// One tick of the clock: the worker handles one task
void cpu_step(CPU *cpu) {
    if (cpu->halted) return;

    // FETCH: read the task form from the filing cabinet
    uint16_t instruction = mem_read16(cpu, cpu->pc);

    // DECODE: read the fields on the task form
    Opcode opcode = DECODE_OPCODE(instruction);
    uint8_t dst   = DECODE_DST(instruction);
    uint8_t src   = DECODE_SRC(instruction);
    uint8_t imm5  = DECODE_IMM5(instruction);
    uint16_t addr = DECODE_ADDR(instruction);

    // Advance checklist BEFORE executing (jumps will override if needed)
    cpu->pc += 2;

    // EXECUTE: do what the task form says
    switch (opcode) {
        case OP_NOP:  break;  // Coffee break

        case OP_LOAD: {  // "Write this number on sticky note #dst" (2-word)
            // The next word on the checklist IS the 16-bit number
            cpu->registers[dst] = mem_read16(cpu, cpu->pc);
            cpu->pc += 2;  // Skip past the number word
            break;
        }

        case OP_MOV:  // "Copy note #src onto note #dst"
            cpu->registers[dst] = cpu->registers[src];
            break;

        case OP_ADD:  // "Add note #dst and note #src, result on note #dst"
            cpu->registers[dst] = alu_execute(cpu, OP_ADD,
                cpu->registers[dst], cpu->registers[src]);
            break;

        case OP_SUB:
            cpu->registers[dst] = alu_execute(cpu, OP_SUB,
                cpu->registers[dst], cpu->registers[src]);
            break;

        case OP_AND:
            cpu->registers[dst] = alu_execute(cpu, OP_AND,
                cpu->registers[dst], cpu->registers[src]);
            break;

        case OP_OR:
            cpu->registers[dst] = alu_execute(cpu, OP_OR,
                cpu->registers[dst], cpu->registers[src]);
            break;

        case OP_XOR:
            cpu->registers[dst] = alu_execute(cpu, OP_XOR,
                cpu->registers[dst], cpu->registers[src]);
            break;

        case OP_NOT:
            cpu->registers[dst] = alu_execute(cpu, OP_NOT,
                cpu->registers[dst], 0);
            break;

        case OP_SHL:
            cpu->registers[dst] = alu_execute(cpu, OP_SHL,
                cpu->registers[dst], imm5);
            break;

        case OP_SHR:
            cpu->registers[dst] = alu_execute(cpu, OP_SHR,
                cpu->registers[dst], imm5);
            break;

        case OP_CMP:  // Use calculator but THROW AWAY the result
            alu_execute(cpu, OP_CMP,
                cpu->registers[dst], cpu->registers[src]);
            break;

        case OP_JMP:  // "Skip to task #N on checklist"
            cpu->pc = addr;
            break;

        case OP_JZ:   // "Skip to task #N IF Zero light is on"
            if (cpu->flags & FLAG_ZERO) cpu->pc = addr;
            break;

        case OP_JNZ:  // "Skip to task #N IF Zero light is off"
            if (!(cpu->flags & FLAG_ZERO)) cpu->pc = addr;
            break;

        case OP_JN:   // "Skip to task #N IF Negative light is on"
            if (cpu->flags & FLAG_NEGATIVE) cpu->pc = addr;
            break;

        case OP_LDR:  // "Go to cabinet folder [src], bring value to note #dst"
            cpu->registers[dst] = mem_read16(cpu, cpu->registers[src]);
            break;

        case OP_STR:  // "File note #src's value into cabinet folder [dst]"
            mem_write16(cpu, cpu->registers[dst], cpu->registers[src]);
            break;

        case OP_PUSH:  // "Place note #dst's value on the inbox tray"
            cpu->sp -= 2;
            mem_write16(cpu, cpu->sp, cpu->registers[dst]);
            break;

        case OP_POP:   // "Take top value from inbox tray, write on note #dst"
            cpu->registers[dst] = mem_read16(cpu, cpu->sp);
            cpu->sp += 2;
            break;

        case OP_CALL: {  // "Bookmark your place, jump to subroutine" (2-word)
            uint16_t target = mem_read16(cpu, cpu->pc);
            cpu->pc += 2;  // Skip past the address word
            // Push return address (current PC = instruction after CALL)
            cpu->sp -= 2;
            mem_write16(cpu, cpu->sp, cpu->pc);
            cpu->pc = target;
            break;
        }

        case OP_RET:   // "Pull bookmark from inbox tray, jump back"
            cpu->pc = mem_read16(cpu, cpu->sp);
            cpu->sp += 2;
            break;

        case OP_HALT:  // "Go home"
            cpu->halted = true;
            printf("Worker went home after %" PRIu64 " tasks.\n", cpu->cycles);
            break;

        default:
            fprintf(stderr, "UNKNOWN TASK 0x%04X at checklist position 0x%04X\n",
                    instruction, cpu->pc - 2);
            cpu->halted = true;
            break;
    }

    cpu->cycles++;
}

// The worker keeps working until they see "Go home"
void cpu_run(CPU *cpu) {
    while (!cpu->halted) {
        cpu_step(cpu);
    }
}