#pragma once

#include "program.hpp"

class Processor
{
public:
  Processor(Program &program);
  void step();
  void info();
  bool complete();

private:
  bool complete_;
  unsigned int PC;       //program counter
  unsigned int stackPtr; //stack pointer register
  unsigned int cycleCount;
  unsigned int memAccessCount;
  vector<int> stack;
  vector<int> registers;
  Program _program;

  void execute();
};