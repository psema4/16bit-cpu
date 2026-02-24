// cpu.h: The office blueprint

#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_REGISTERS  8      // 8 sticky notes on the desk
#define MEMORY_SIZE    65536  // 64 KB filing cabinet

// Status board lights
#define FLAG_ZERO      (1 << 0)  // "The answer was zero"
#define FLAG_NEGATIVE  (1 << 1)  // "The answer was negative"
#define FLAG_OVERFLOW  (1 << 2)  // "The numbers wrapped around"

typedef struct {
    uint16_t registers[NUM_REGISTERS];  // The 8 sticky notes (R0–R7)
    uint16_t pc;       // Task checklist position
    uint16_t sp;       // Inbox tray position (stack pointer)
    uint16_t flags;    // Status board (Z, N, O lights)
    uint8_t  memory[MEMORY_SIZE];  // The filing cabinet
    bool     halted;   // Has the worker gone home?
    uint64_t cycles;   // How many tasks completed
} CPU;

#endif