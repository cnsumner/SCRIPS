#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
using std::dec;
using std::hex;
using std::setfill;
using std::setw;
using std::left;
using std::right;
#include <sstream>
using std::stringstream;


#include "processor.hpp"

Processor::Processor(Program &program)
{
    complete_ = false;
    PC = program._entryPoint;
    stackPtr = 1027; //start stack at end of memory
    stack = vector<int>(1028, 0);
    registers = vector<int>(8, 0);
    cycleCount = 0;
    _program = program;
}

void Processor::step()
{
    execute();
}

void Processor::execute()
{
    //note: casting pointer to char* and adding PC then casting back to int*
    //          is the same as casting to int* and adding PC/4. this is
    //          simply done to preserve the PC + 4 idea from
    //          word alligned memory
	int opcode = _program._instructions[PC].opcode;
	int reg = _program._instructions[PC].reg;
    int param = _program._instructions[PC].param;

    if (opcode == 0x00) //LOAD reg, addr
    {
        registers[reg] = _program._dataMemory[param];
    }
    else if (opcode == 0x01) //LOADI reg, addr
    {
        int addr = _program._dataMemory[param];
        registers[reg] = _program._dataMemory[addr];
    }
    else if (opcode == 0x02) //STORE reg, addr
    {
        _program._dataMemory[param] = registers[reg];
    }
    else if (opcode == 0x03) //STOREI reg, addr
    {
        int addr = _program._dataMemory[param];
        _program._dataMemory[addr] = registers[reg];
    }
    else if (opcode == 0x04) //ADD reg, addr
    {
        registers[reg] = registers[reg] + _program._dataMemory[param];
    }
    else if (opcode == 0x05) //ADDI reg, const
    {
		registers[reg] = registers[reg] + param;
    }
    else if (opcode == 0x06) //AND reg, addr
    {
        registers[reg] = registers[reg] & _program._dataMemory[param];
    }
    else if (opcode == 0x07) //OR reg, addr
    {
        registers[reg] = registers[reg] | _program._dataMemory[param];
    }
    else if (opcode == 0x08) //NOT reg
    {
        registers[reg] = ~registers[reg];
    }
    else if (opcode == 0x09) //XOR reg, addr
    {
        registers[reg] = registers[reg] ^ _program._dataMemory[param];
    }
    else if (opcode == 0x18) //move reg2, reg2
    {
        registers[reg] = registers[param];
    }
    else if (opcode == 0x0A) //JUMP const
    {
        PC = param;
        cycleCount++;
        return;
    }
    else if (opcode == 0x0B) //BZERO reg, addr
    {
        PC = (registers[reg] == 0 ? param : PC + 1); //PC + 4
        cycleCount++;
        return;
    }
    else if (opcode == 0x0C) //SEQ	reg, addr
    {
        registers[reg] = (registers[reg] == _program._dataMemory[param] ? 1 : 0);
    }
    else if (opcode == 0x0D) //SNE	addr
    {
        registers[reg] = (registers[reg] != _program._dataMemory[param] ? 1 : 0);
    }
    else if (opcode == 0x0E) //SGT	addr
    {
        registers[reg] = (registers[reg] > _program._dataMemory[param] ? 1 : 0);
    }
    else if (opcode == 0x0F) //SLT	addr
    {
        registers[reg] = (registers[reg] < _program._dataMemory[param] ? 1 : 0);
    }
    else if (opcode == 0x10) //SGE	addr
    {
        registers[reg] = (registers[reg] >= _program._dataMemory[param] ? 1 : 0);
    }
    else if (opcode == 0x11) //SLE	addr
    {
        registers[reg] = (registers[reg] <= _program._dataMemory[param] ? 1 : 0);
    }
    else if (opcode == 0x12) //PUSH reg
    {
        stackPtr--; //stackPtr - 4
        stack[stackPtr] = registers[reg];
    }
    else if (opcode == 0x13) //POP reg
    {
        registers[reg] = stack[stackPtr];
        stackPtr++; //stackPtr + 4
    }
    else if (opcode == 0x14) //CALL addr
    {
        stackPtr--;
        stack[stackPtr] = PC + 1; //PC + 4
        PC = param;
        cycleCount++;
        return;
    }
    else if (opcode == 0x15) //RET
    {
        PC = stack[stackPtr];
        stackPtr++; //stackPtr + 4
        cycleCount++;
        return;
    }
    else if (opcode == 0x17)
    {
        if (_program._instructions[PC].mnemonic == "exit")
            complete_ = true;
    }

    PC++; //PC + 4
    
    if (opcode != 0x17)
        cycleCount++;
}

void Processor::info()
{
    cout << "|-------------------------|" << endl;
    cout << "| PC: " << setfill(' ') << left << setw(19) << dec << PC << " |" << endl;
    cout << "|-------------------------|" << endl;
    cout << "| Stack ptr: " << setfill(' ') << left << setw(12) << dec << stackPtr << " |" << endl;
    cout << "|-------------------------|" << endl;
    cout << "| Accumulator: " << left << setw(sizeof(int)*2 + 2) << setfill(' ') << dec << registers[0] << " |" << endl;
    cout << "|-------------------------|" << endl;
    
    int prevInstructions = (PC > 2 ? PC - 3 : 0);
    int futureInstructions = (prevInstructions + 6 < _program._instructions.size() ? prevInstructions + 6 : _program._instructions.size() - 1);

    for (int i = prevInstructions; i <= futureInstructions; ++i)
    {
        Instruction instr = _program._instructions[i];
        cout << (i == PC ? "|* " : "|  ") << setfill(' ') << left << setw(7) << instr.mnemonic << " " << left << setfill(' ') << setw(sizeof(int)*2 + 2) << dec << instr.param << "     |" << endl;
    }
    cout << "|-------------------------|" << endl;
    cout << "| Cycles: " << left << setw(15) << setfill(' ') << dec << cycleCount << " |" << endl;
    cout << "|-------------------------|" << endl;
}

bool Processor::complete()
{
    return complete_;
}