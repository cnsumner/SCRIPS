#pragma once

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <map>
using std::map;

struct Instruction
{
  string mnemonic;
  int opcode;
  int reg;
  int param;
};

class Program
{
  public:
    vector<Instruction> _instructions;
    vector<int> _dataMemory;
    int _entryPoint;
    int _endOfMemory;
    map<string, int> _labels;

    Program();
};