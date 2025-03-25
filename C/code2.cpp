#include <iostream>
#include <vector>
#include <cstdint>

#define NUM_REGS 16
#define INSTR_MEM_SIZE 128
#define DATA_MEM_SIZE 100

struct PipelineStage
{
    uint32_t instr = 0;
    uint8_t opcode = 0;
    uint8_t operand1 = 0;
    uint8_t operand2 = 0;
    uint8_t operand3 = 0;
    bool valid = false;
};

class Processor
{
public:
    Processor() : pc(0), cycle(0), temp_result(0)
    {
        reg.resize(NUM_REGS, 0);
        instr_mem.resize(INSTR_MEM_SIZE, 0);
        data_mem.resize(DATA_MEM_SIZE, 0);
    }

    void load_program(const std::vector<uint32_t> &program)
    {
        for (size_t i = 0; i < program.size() && i < INSTR_MEM_SIZE; ++i)
        {
            instr_mem[i] = program[i];
        }
    }

    void run_pipeline(size_t steps)
    {
        for (size_t i = 0; i < steps; ++i)
        {
            cycle++;
            write_back();
            memory_access();
            execute();
            decode();
            fetch();
        }
    }

    void fetch()
    {
        if (pc < INSTR_MEM_SIZE)
        {
            IF.instr = instr_mem[pc++];
            IF.valid = true;
        }
        else
        {
            IF.valid = false;
        }
    }

    void decode()
    {
        if (!IF.valid)
        {
            ID.valid = false;
            return;
        }
        uint32_t instr = IF.instr;
        ID.instr = instr;
        ID.opcode = static_cast<uint8_t>((instr >> 24) & 0xFF);
        ID.operand1 = static_cast<uint8_t>((instr >> 16) & 0xFF);
        ID.operand2 = static_cast<uint8_t>((instr >> 8) & 0xFF);
        ID.operand3 = static_cast<uint8_t>(instr & 0xFF);
        ID.valid = true;
    }

    void execute()
    {
        if (!ID.valid)
        {
            EX.valid = false;
            return;
        }
        EX = ID;
        switch (EX.opcode)
        {
        case 0x01:
            temp_result = reg[EX.operand2] + reg[EX.operand3];
            break;
        case 0x02:
            temp_result = reg[EX.operand2] - reg[EX.operand3];
            break;
        case 0x03:
            temp_result = reg[EX.operand2] & reg[EX.operand3];
            break;
        case 0x04:
            temp_result = reg[EX.operand2] | reg[EX.operand3];
            break;
        case 0x05:
            temp_result = reg[EX.operand2] ^ reg[EX.operand3];
            break;
        case 0x06:
            temp_result = reg[EX.operand2] + EX.operand3;
            break;
        case 0x07:
            temp_result = reg[EX.operand2] + EX.operand3;
            break;
        case 0x08:
            if (reg[EX.operand1] == reg[EX.operand2])
            {
                pc += EX.operand3;
            }
            break;
        default:
            break;
        }
        EX.valid = true;
    }

    void memory_access()
    {
        if (!EX.valid)
        {
            MEM.valid = false;
            return;
        }
        MEM = EX;
        if (MEM.opcode == 0x06 && temp_result < DATA_MEM_SIZE)
        {
            MEM.instr = data_mem[temp_result];
        }
        else if (MEM.opcode == 0x07 && temp_result < DATA_MEM_SIZE)
        {
            data_mem[temp_result] = static_cast<uint8_t>(reg[MEM.operand1]);
        }
        else
        {
            MEM.instr = temp_result;
        }
        MEM.valid = true;
    }

    void write_back()
    {
        if (!MEM.valid)
            return;
        switch (MEM.opcode)
        {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
            if (MEM.operand1 < NUM_REGS)
            {
                reg[MEM.operand1] = MEM.instr;
            }
            break;
        default:
            break;
        }
    }

    void print_registers() const
    {
        std::cout << "Registers:" << std::endl;
        for (int i = 0; i < NUM_REGS; ++i)
            std::cout << "R" << i << ": " << reg[i] << " ";
        std::cout << std::endl;
    }

    void print_memory() const
    {
        std::cout << "Data Memory:" << std::endl;
        for (int i = 0; i < 20; ++i)
            std::cout << "M[" << i << "]: " << static_cast<int>(data_mem[i]) << " ";
        std::cout << std::endl;
    }

private:
    std::vector<uint32_t> reg;
    std::vector<uint32_t> instr_mem;
    std::vector<uint8_t> data_mem;
    uint32_t pc;
    uint64_t cycle;
    int temp_result;

    PipelineStage IF, ID, EX, MEM;
};

// Testbench
int main()
{
    Processor p;

    std::vector<uint32_t> program = {
        0x01010203, // ADD R1, R2, R3
        0x01020304, // ADD R2, R3, R4
        0x02040506, // SUB R4, R5, R6
        0x03070809, // AND R7, R8, R9
        0x04101112, // OR R1, R10, R11
        0x05131415, // XOR R3, R14, R15
        0x06070108, // LD R7, R1, 8
        0x07151209, // ST R15, R12, 9
        0x0834030D, // BEQ R4, R3, 13
        0x00000000  // NOP
    };

    p.load_program(program);
    p.run_pipeline(program.size() + 4); // +4 cycles to flush pipeline

    p.print_registers();
    p.print_memory();

    return 0;
}
