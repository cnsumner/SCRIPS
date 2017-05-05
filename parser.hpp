#pragma once

#include <fstream>
#include <utility>
#include <string>
#include <vector>

#include "program.hpp"

using std::string;
using std::vector;
using std::ifstream;
class Parser
{
public:
  Parser(string filename);
  bool ok();
  int parse(Program &program);

private:
  vector<string> _instructions;

  int encode(string mnemonic);
};