#include <iostream>
#include <vector>
#include <cstdint>

#define NUM_REGS 16
#define INSTR_MEM_SIZE 128
#define DATA_MEM_SIZE 100

class Processor {
public:
Processor() : pc(0), cycle(0) {
reg.resize(NUM_REGS, 0);
instr_mem.resize(INSTR_MEM_SIZE, 0);
data_mem.resize(DATA_MEM_SIZE, 0);
}
void fetch() {
    
if (pc < INSTR_MEM_SIZE) {
current_instr = instr_mem[pc];
pc++;
}
printf("break point 1 \n");
}
void decode() {
opcode = (current_instr >> 24) & 0xFF;
operand1 = (current_instr >> 16) & 0xFF;
operand2 = (current_instr >> 8) & 0xFF;
operand3 = current_instr & 0xFF;
printf("break point 2 \n");

}
void execute() {
switch (opcode) {
case 0x00: // NOP
break;
case 0x01: // ADD
reg[operand1] = reg[operand2] + reg[operand3]; //0x 01 02 = 03 + 04, // ADD R1, R2, R3
break;
case 0x02: // SUB
reg[operand1] = reg[operand2] - reg[operand3];
break;
case 0x03: // AND
reg[operand1] = reg[operand2] & reg[operand3];
break;
case 0x04: // OR
reg[operand1] = reg[operand2] | reg[operand3];
break;
case 0x05: // XOR
reg[operand1] = reg[operand2] ^ reg[operand3];
break;
case 0x06: // LD
reg[operand1] = data_mem[reg[operand2] + operand3]; //0x 06 R7, R1, 8, // LD R7, R1, 8
break;
case 0x07: // ST
data_mem[reg[operand2] + operand3] = reg[operand1];
break;
case 0x08: // BEQ //0x 08 34 1213 // BEQ R4, R3, 13
if (reg[operand1] == reg[operand2]) {
pc += operand3;
}
break;
default:
std::cerr << "Invalid opcode: " << opcode << std::endl;
break;
}
printf("break point 3 \n");

}
void execute_pipeline() {
fetch();
decode();
execute();
printf("break point 4 \n");

}
void load_program(const std::vector<uint32_t>& program) {
for (size_t i = 0; i < program.size(); ++i) {
instr_mem[i] = program[i];
}
printf("break point 5 \n");

}
void print_registers() const {
std::cout << "Registers:\t";
for (size_t i = 0; i < NUM_REGS; ++i) {
std::cout << "R" << i << ": " << reg[i] << " ";
}
std::cout << std::endl;
}
void print_memory() const {
std::cout << "Data Memory:\t";
for (size_t i = 0; i < DATA_MEM_SIZE; ++i) {
std::cout << "M[" << i << "]: " << static_cast<int>(data_mem[i]) << "\t";
}
std::cout << std::endl;
}
private:
std::vector<uint32_t> reg;
std::vector<uint32_t> instr_mem;
std::vector<uint8_t> data_mem;
uint32_t pc;
uint32_t current_instr;
uint8_t opcode;
uint8_t operand1;
uint8_t operand2;
uint8_t operand3;
uint64_t cycle;
};
int main() {
Processor p;
// Sample program: Adds contents of R2 and R3, stores result in R1
std::vector<uint32_t> program = {
0x01010203, // ADD R1, R2, R3
0x01020304, // ADD R2, R3, R4
0x02040506, // SUB R4, R5, R6
0x03070809, // AND R7, R8, R9
0x04101112, // OR R1, R10, R11
0x05131415, // XOR R3, R14, R15
0x06070108, // LD R7, R1, 8
0x07151219, // ST R15, R12, 9
0x08341213, // BEQ R4, R3, 13
0x00000000 // NOP
};
p.load_program(program);
// Execute pipeline for each instruction
for (size_t i = 0; i < program.size(); ++i) {
p.execute_pipeline();
}
// Print register and memory contents
p.print_registers();
p.print_memory();
return 0;
}