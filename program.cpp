#include "program.hpp"

Program::Program()
{
	_instructions = vector<Instruction>();
	_dataMemory = vector<int>(4096, 0);
	_entryPoint = 0;
	_endOfMemory = 0;
	_labels = map<string, int>();
}