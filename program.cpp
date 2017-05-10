#include "program.hpp"

Program::Program()
{
	_instructions = vector<Instruction>();
	_dataMemory = vector<int>(8192, 0);
	_entryPoint = 0;
	_endOfMemory = 0;
	_labels = map<string, int>();
}