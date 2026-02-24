# 16-bit CPU emulator from scratch in pure C

## File Structure

```
16bit-cpu/
├── opcodes.h        — The 16 task types
├── cpu.h            — The office blueprint
├── instruction.h    — Task form encoding/decoding
├── alu.c            — The calculator + status board
├── memory.c         — The filing cabinet
├── cpu_execute.c    — The worker's daily routine
├── assembler.c      — The secretary
└── main.c           — Hire the worker, run the demo
```

---

### Compile & Run

```bash
gcc -o cpu16 main.c cpu_execute.c alu.c assembler.c memory.c -Wall -std=c11
./cpu16
```

---

### Expected Output

```bash
=== 16-BIT CPU EMULATOR ===
=== The Office Worker Simulation ===

...
Worker went home after 411 tasks.

=== DESK STATUS ===
  Sticky note R0 = 5050 (0x13BA)
  ...
  Sticky note R5 = 5050 (0x13BA)
  ...

=== VERIFICATION ===
  R0 (register)    = 5050 ✓
  R5 (from memory) = 5050 ✓
  Memory[1000]     = 5050 ✓

✓ SUCCESS! All three match: 1+2+...+100 = 5050
```
