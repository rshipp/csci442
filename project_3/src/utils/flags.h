#pragma once
#include <string>

struct Options {
  bool verbose = false;
  bool per_thread = false;
  std::string algorithm;
  std::string in_file;
};

Options parse_args(int argc, char **argv);

void usage(char* pname);
