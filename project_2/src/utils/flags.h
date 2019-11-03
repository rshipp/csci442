#include <string>

struct Options {
  std::string sort;
  int delay;
};

Options parse_args(int argc, char **argv);

void usage(char* pname);
