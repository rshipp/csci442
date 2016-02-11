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
  if(argv.size() == 2) {
    d = opendir(argv[1].c_str());
  } else if (argv.size() == 1) {
    d = opendir(getcwd(NULL, 0));
  } else {
    cerr << argv[0] << ": ls: too many arguments" << endl;
    return 1;
  }
  if(!d) {
    perror(argv[0].c_str());
    return errno;
  }
  struct dirent * file;
  while((file = readdir(d)) != NULL) {
    cout << (*file).d_name << '\n';
  }
  return 0;
}


int Shell::com_cd(vector<string>& argv) {
  if(argv.size() == 2) {
    if(chdir(argv[1].c_str())) {
      perror(argv[0].c_str());
      return errno;
    }
  } else if (argv.size() == 1) {
    if(chdir(getenv("HOME"))) {
      perror(argv[0].c_str());
      return errno;
    }
  } else {
    cerr << argv[0] << ": cd: too many arguments" << endl;
    return 1;
  }
  return 0;
}


int Shell::com_pwd(vector<string>& argv) {
  if(argv.size() > 1) {
    cerr << argv[0] << ": pwd: too many arguments" << endl;
    return 1;
  }
  cout << getcwd(NULL, 0) << endl;
  return 0;
}


int Shell::com_alias(vector<string>& argv) {
  if(argv.size() == 2) {
    string cmd = argv[1];
    int split = cmd.find('=');
    if(split > 0) {
      this->aliases[cmd.substr(0,split)] = cmd.substr(split+1,cmd.size());
    } else {
      return 1;
    }
  } else if (argv.size() == 1) {
    map<string,string>::iterator it;
    for(it = this->aliases.begin(); it != this->aliases.end(); it++) {
      cout << (*it).first << "=" << (*it).second << endl;
    }
  } else {
    cerr << argv[0] << ": alias: too many arguments" << endl;
    return 1;
  }
  return 0;
}


int Shell::com_unalias(vector<string>& argv) {
  if(argv.size() == 2 && argv[1] == string("-a")) {
    this->aliases.clear();
  } else if (argv.size() == 2) {
    this->aliases.erase(argv[1]);
  } else {
    cerr << argv[0] << ": unalias: too many arguments" << endl;
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
  if(argv.size() > 1) {
    cerr << argv[0] << ": history: too many arguments" << endl;
    return 1;
  }

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
