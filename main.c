// main.c: Hire the worker, give them a job

#include <stdio.h>
#include <inttypes.h>
#include "cpu.h"
#include "instruction.h"

extern void cpu_init(CPU *cpu);
extern void cpu_run(CPU *cpu);
extern uint16_t mem_read16(CPU *cpu, uint16_t address);
extern void mem_write16(CPU *cpu, uint16_t address, uint16_t value);
extern int assemble(const char *source, uint16_t *output, int max);

void load_program(CPU *cpu, uint16_t *words, int count) {
    for (int i = 0; i < count; i++)
        mem_write16(cpu, i * 2, words[i]);
    printf("Filed %d words into the cabinet (%d bytes)\n", count, count * 2);
}

void cpu_dump(CPU *cpu) {
    printf("\n=== DESK STATUS ===\n");
    for (int i = 0; i < NUM_REGISTERS; i++)
        printf("  Sticky note R%d = %d (0x%04X)\n", i, cpu->registers[i], cpu->registers[i]);
    printf("  Checklist position: 0x%04X\n", cpu->pc);
    printf("  Inbox tray position: 0x%04X\n", cpu->sp);
    printf("  Status board: [%c%c%c]\n",
        (cpu->flags & FLAG_OVERFLOW) ? 'O' : '-',
        (cpu->flags & FLAG_NEGATIVE) ? 'N' : '-',
        (cpu->flags & FLAG_ZERO)     ? 'Z' : '-');
    printf("  Tasks completed: %" PRIu64 "\n", cpu->cycles);
}

int main(void) {
    printf("=== 16-BIT CPU EMULATOR ===\n");
    printf("=== The Office Worker Simulation ===\n\n");

    CPU cpu;
    cpu_init(&cpu);

    const char *program =
        "; Sum of 1 to 100: answer ends up on sticky note R0\n"
        "; Demonstrates: full LOAD, memory access, stack operations\n"
        "LOAD R0, 0\n"
        "LOAD R1, 100\n"
        "LOAD R2, 1\n"
        "LOAD R3, 0\n"
        "LOAD R4, 1000\n"
        "loop:\n"
        "ADD R0, R1\n"
        "SUB R1, R2\n"
        "CMP R1, R3\n"
        "JNZ loop\n"
        "STR R4, R0\n"
        "PUSH R0\n"
        "LOAD R0, 0\n"
        "POP R0\n"
        "LDR R5, R4\n"
        "HALT\n";

    printf("Checklist:\n%s\n", program);

    uint16_t machine_code[512];
    int count = assemble(program, machine_code, 512);

    printf("Task forms (machine code):\n");
    for (int i = 0; i < count; i++)
        printf("  Word [%02d] @ 0x%04X: 0x%04X\n", i, i * 2, machine_code[i]);

    load_program(&cpu, machine_code, count);
    printf("\n=== Worker starts... ===\n");
    cpu_run(&cpu);
    cpu_dump(&cpu);

    // Verify results
    printf("\n=== VERIFICATION ===\n");
    uint16_t mem_result = mem_read16(&cpu, 1000);
    printf("  R0 (register)    = %d %s\n", cpu.registers[0], cpu.registers[0] == 5050 ? "✓" : "✗");
    printf("  R5 (from memory) = %d %s\n", cpu.registers[5], cpu.registers[5] == 5050 ? "✓" : "✗");
    printf("  Memory[1000]     = %d %s\n", mem_result, mem_result == 5050 ? "✓" : "✗");

    if (cpu.registers[0] == 5050 && cpu.registers[5] == 5050 && mem_result == 5050)
        printf("\n✓ SUCCESS! All three match: 1+2+...+100 = 5050\n");
    else
        printf("\n✗ ERROR: something went wrong\n");

    return 0;
}