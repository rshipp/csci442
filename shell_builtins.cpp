/**
 * This file contains the implementations of the builtin functions provided by
 * the shell (or will, once you've finished implementing them all).
 */

#include "shell.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <readline/history.h>
#include <readline/readline.h>

using namespace std;


int Shell::com_ls(vector<string>& argv) {
  DIR* d;
  if(argv.size() > 1) {
    d = opendir(argv[1].c_str());
  } else {
    d = opendir(getcwd(NULL, 0));
  }
  struct dirent * file;
  while((file = readdir(d)) != NULL) {
    cout << (*file).d_name << '\n';
  }
  return 0;
}


int Shell::com_cd(vector<string>& argv) {
  if(argv.size() > 1) {
    if(chdir(argv[1].c_str())) {
      perror(argv[0].c_str());
      return errno;
    }
  } else {
    if(chdir(getenv("HOME"))) {
      perror(argv[0].c_str());
      return errno;
    }
  }
  return 0;
}


int Shell::com_pwd(vector<string>& argv) {
  cout << getcwd(NULL, 0) << endl;
  return 0;
}


int Shell::com_alias(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  if(argv.size() > 1) {
    string cmd = argv[1];
    int split = cmd.find('=');
    if(split > 0) {
      this->aliases[cmd.substr(0,split)] = cmd.substr(split+1,cmd.size());
    } else {
      return 1;
    }
  } else {
    map<string,string>::iterator it;
    for(it = this->aliases.begin(); it != this->aliases.end(); it++) {
      cout << (*it).first << "=" << (*it).second << endl;
    }
  }
  return 0;
}


int Shell::com_unalias(vector<string>& argv) {
  if(argv.size() > 1) {
    this->aliases.erase(string(argv[1]));
  } else {
    return 1;
  }
  return 0;
}


int Shell::com_echo(vector<string>& argv) {
  for (size_t i=1; i<argv.size(); i++) {
    cout << argv[i] << ' ';
  }
  cout << endl;
  return 0;
}


int Shell::com_history(vector<string>& argv) {
  HIST_ENTRY ** histlist = history_list();
  if(!histlist) {
    return 0;
  }
  int i = 0;
  while(*histlist) {
    cout << ++i << ' ' << (**histlist).line << endl;
    histlist++;
  }
  return 0;
}


int Shell::com_exit(vector<string>& argv) {
  exit(0);
  return 0;
}
