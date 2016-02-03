/**
 * This file contains implementations of the functions that provide
 * tab-completion for the shell.
 *
 * You will need to finish the implementations of the completion functions,
 * though you're spared from implementing the high-level readline callbacks with
 * their weird static variables...
 */

#include "shell.h"
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;


void Shell::get_env_completions(const char* text, vector<string>& matches) {
  string word = string(text + 1);
  int i = 0;
  char* compare;
  string str;
  int split;

  // ENV vars
  while((compare = environ[i++])) {
    str = string(compare);
    if(str.substr(0, word.size()) == word.substr(0, word.size())) {
      split = str.find('=');
      if(split > 0) {
        matches.insert(matches.end(), "$" + str.substr(0,split));
      }
    }
  }
  // local vars
  map<string,string>::iterator it;
  for(it = this->localvars.begin(); it != this->localvars.end(); it++) {
    if((*it).first.substr(0, word.size()) == word.substr(0, word.size())) {
      matches.insert(matches.end(), "$" + (*it).first);
    }
  }

}


void Shell::get_command_completions(const char* text, vector<string>& matches) {
  string word = string(text);
  string str;
  string unsplit_path = string(getenv("PATH") + NULL);
  vector<string> path;
  while(unsplit_path.find(':') != string::npos) {
    path.insert(path.end(), unsplit_path.substr(0, unsplit_path.find(':')));
    unsplit_path = unsplit_path.substr(unsplit_path.find(':')+1, unsplit_path.size());
  }
  path.insert(path.end(), unsplit_path);

  DIR* d;
  vector<string>::iterator it;
  for(it = path.begin(); it != path.end(); it++) {
    d = opendir((*it).c_str());
    struct dirent * file;
    while((file = readdir(d)) != NULL) {
      if((*file).d_type == DT_REG || (*file).d_type == DT_LNK) {
        str = string((*file).d_name);

        if(str.substr(0, word.size()) == word.substr(0, word.size())) {
          matches.insert(matches.end(), str);
        }
      }
    }
  }
}


char** Shell::word_completion(const char* text, int start, int end) {
  char** matches = NULL;

  if (text[0] == '$') {
    matches = rl_completion_matches(text, env_completion_generator);
  } else if (start == 0) {
    matches = rl_completion_matches(text, command_completion_generator);
  } else {
    // We get directory matches for free (thanks, readline!).
  }

  return matches;
}


char* Shell::env_completion_generator(const char* text, int state) {
  // A list of all the matches.
  // Must be static because this function is called repeatedly.
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches.
  if (state == 0) {
    getInstance().get_env_completions(text, matches);
  }

  // Return a single match (one for each time the function is called).
  return pop_match(matches);
}


char* Shell::command_completion_generator(const char* text, int state) {
  // A list of all the matches.
  // Must be static because this function is called repeatedly.
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches.
  if (state == 0) {
    getInstance().get_command_completions(text, matches);
  }

  // Return a single match (one for each time the function is called).
  return pop_match(matches);
}


char* Shell::pop_match(vector<string>& matches) {
  if (matches.size() > 0) {
    const char* match = matches.back().c_str();

    // Delete the last element.
    matches.pop_back();

    // We need to return a copy, because readline deallocates when done.
    char* copy = (char*) malloc(strlen(match) + 1);
    strcpy(copy, match);

    return copy;
  }

  // No more matches.
  return NULL;
}
