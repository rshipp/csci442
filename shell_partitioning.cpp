/**
 * Contains the implementation of functions declared in command.h.
 */

#include "shell_partitioning.h"
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;


bool partition_tokens(vector<string> tokens, vector<command_t>& commands) {
  ParserState state = INITIAL;
  string token;
  command_t command;
  command.input_type = READ_FROM_STDIN;
  command.output_type = WRITE_TO_STDOUT;
  commands.insert(commands.end(), command);

  vector<string>::iterator it;
  for(it = tokens.begin(); it != tokens.end(); it++) {
    token = string(*it);

    if(token == string("|")) {
      if(state != CLEAN || commands.back().output_type != WRITE_TO_STDOUT) {
        // parse error
        return false;
      }
      state = WAS_PIPE;
      commands.back().output_type = WRITE_TO_PIPE;
      // new command
      command_t command;
      command.input_type = READ_FROM_PIPE;
      command.output_type = WRITE_TO_STDOUT;
      commands.insert(commands.end(), command);
    } else if(token.find("<") != string::npos) {
      // input redirection
      if(state != CLEAN || commands.back().input_type != READ_FROM_STDIN) {
        // parse error
        return false;
      }
      state = IS_INFILE;
      if(token == string("<")) {
          commands.back().input_type = READ_FROM_FILE;
      } else {
          // parse error
          return false;
      }

    } else if(token.find(">") != string::npos) {
      // outpyt redirection
      if(state != CLEAN || commands.back().output_type != WRITE_TO_STDOUT) {
        // parse error
        return false;
      }
      state = IS_OUTFILE;
      if(token == string(">")) {
          commands.back().output_type = WRITE_TO_FILE;
      } else if(token == string(">>")) {
          commands.back().output_type = APPEND_TO_FILE;
      } else {
          // parse error
          return false;
      }
    } else if(state == IS_INFILE) {
      commands.back().infile = token;
      state = CLEAN;
    } else if(state == IS_OUTFILE) {
      commands.back().outfile = token;
      state = CLEAN;
    } else {
      // argument
      state = CLEAN;
      commands.back().argv.insert(commands.back().argv.end(), token);
    }
  }

  return state == CLEAN;
}


// Feel free to ignore everything below here. It's just code to allow you to
// cout a command in a nice, pretty format. =)


const char* input_types[] = {
  "READ_FROM_STDIN",
  "READ_FROM_FILE",
  "READ_FROM_PIPE"
};


const char* output_types[] = {
  "WRITE_TO_STDOUT",
  "WRITE_TO_PIPE",
  "WRITE_TO_FILE",
  "APPEND_TO_FILE"
};


ostream& operator <<(ostream& out, const command_t& cmd) {
  copy(cmd.argv.begin(), cmd.argv.end(), ostream_iterator<string>(out, " "));

  out << "\n    input:   " << input_types[cmd.input_type]
      << "\n    output:  " << output_types[cmd.output_type]
      << "\n    infile:  " << cmd.infile
      << "\n    outfile: " << cmd.outfile;

  return out;
}
