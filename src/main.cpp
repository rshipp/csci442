#include <iostream>
#include "flags.h"
#include "config.h"

using namespace std;

int main(int argc, char** argv) {
  Options options = parse_args(argc, argv);
  Config config = parse_config(options.in_file);


  return 0;
}
