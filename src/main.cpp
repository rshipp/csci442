/**
 * This file contains the main() function that drives the simulation. You'll
 * need to add logic to this file to create a Simulation instance and invoke its
 * run() method.
 */

#include <cstdlib>
#include "flag_parser/flag_parser.h"

using namespace std;


/**
 * The main entry point to the simulation.
 */
int main(int argc, char** argv) {
  FlagOptions options;
  if (parse_flags(argc, argv, options)) {
    
  } else {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
