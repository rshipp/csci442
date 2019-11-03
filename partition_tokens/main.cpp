/**
 * A simple program that reads from STDIN and calls 'partition_tokens' on the
 * input. If the partitioning succeeds, it prints the resulting commands.
 *
 * See the makefile for instructions on how to run this program.
 */

#include <iostream>
#include <cstdlib>
#include <iterator>
#include "command.h"

using namespace std;

int main() {
  // Convert STDIN to a vector of tokens (the same format your shell uses).
  vector<string> tokens = vector<string>(
      istream_iterator<string>(cin),
      istream_iterator<string>());

  // Declare an empty vector into which we'll put the partitioned commands.
  vector<command_t> cmds;

  // Attempt to partition the commands. If it doesn't work, exit.
  if (!partition_tokens(tokens, cmds)) {
    return EXIT_FAILURE;
  }

  // Print out the results.
  for (size_t i = 1; i <= cmds.size(); i++) {
    cout << i << ": " << cmds[i - 1] << "\n\n";
  }

  cout << "Number of commands: " << cmds.size() << endl;
  return EXIT_SUCCESS;
}
