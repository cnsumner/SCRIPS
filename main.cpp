/* Instruction set (22 instructions)

#	0b			0x		Instr.	Args
-----------------------------------------------------------
0	0b00000		0x00	LOAD 	reg, addr
1	0b00001		0x01	LOADI 	reg, addr
2	0b00010		0x02	STORE 	reg, addr
3	0b00011		0x03	STOREI 	reg, addr
4	0b00100		0x04	ADD 	reg, addr
5	0b00101		0x05	ADDI 	reg, const
6	0b00110		0x06	AND 	reg, addr
7	0b00111		0x07	OR 		reg, addr
8	0b01000		0x08	NOT		reg
9	0b01001		0x09	XOR 	reg, addr
24  0b11000		0x18	MOVE	reg1, reg2
10	0b01010		0x0A	JUMP 	const
11	0b01011		0x0B	BZERO	reg, const
12	0b01100		0x0C	SEQ		reg, addr
13	0b01101		0x0D	SNE		reg, addr
14	0b01110		0x0E	SGT		reg, addr
15	0b01111		0x0F	SLT		reg, addr
16	0b10000		0x10	SGE		reg, addr
17	0b10001		0x11	SLE		reg, addr
18	0b10010		0x12	PUSH	reg
19	0b10011		0x13	POP		reg
20	0b10100		0x14	CALL	const
21	0b10101		0x15	RET
22	0b10110		0x16	array and var
23	0b10111		0x17	labels
-----------------------------------------------------------

Instruction anatomy (bits)
-------------------------
[opcode(5)][Arguments(27)]

Processor anatomy
-------------------------
Program Counter (PC) 	32 bits
Accumulator				32 bits
Memory					(32 bits)*(2^32) = 137,438,953,472 bits = 17,179,869,184 bytes = 17 MB

*/

#include <iostream>
#include <string>

#include "parser.hpp"
#include "processor.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;

int main(int argc, char *argv[])
{
	bool debug = false;
	if (argc != 2 && argc != 3)
	{
		cout << "Incorrect arguments. Please specify input file. Optionally specify debug mode.\n";
		cout << "EXAMPLE: snaps test.asm" << endl << endl;
		cout << "EXAMPLE: snaps test.asm true" << endl << endl;
		return 0;
	}

	if (argc == 3)
	{
		string d = argv[2];
		if (d == "true")
			debug = true;
	}

	string filename = argv[1];

	Parser p = Parser(filename);

	if (p.ok())
	{
		Program prog = Program();
		
		int err = p.parse(prog);

		if (err == 0)
		{
			Processor proc = Processor(prog);
			cout << endl;

			int run = 1;
			
			while (!proc.complete())
			{
				if (debug)
				{
					run--;
					proc.info();
					cout << endl;
					cout << "Press enter to step once, or enter a number to step that many times..." << endl;
				}

				if (run < 1 && debug)
				{
					run = 1;
					string s;
					getline(cin, s);
					if (s.size() > 0)
						run = atoi(s.c_str());
				}
				proc.step();
			}

			proc.info();
		}
	} else
	{
		cout << "Error opening file." << endl;
	}
}