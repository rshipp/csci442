/**
 * This file contains implementations of the functions that are responsible for
 * executing commands that are not builtins.
 *
 * Much of the code you write will probably be in this file. Add helper methods
 * as needed to keep your code clean. You'll lose points if you write a single
 * monolithic function!
 */

#include "shell.h"
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

using namespace std;


int Shell::execute_external_command(vector<string>& tokens) {
  int pid; // process ID

  // Attempt to fork off a new process.
  if ((pid = fork()) == -1) {
    perror("fork failed");
    return 1;
  }

  // fork() will return two different values on success, depending on whether
  // the current process is the parent or the child.

  // fork() returns 0 in the child process
  if (pid == 0) {
    // Execute the given command.
    char** command;
    int i = 0;
    vector<string>::iterator it;
    for(it = tokens.begin(); it != tokens.end(); it++) {
      command[i++] = (char*)(*it).c_str();
    }
    command[i] = (char*)0;
    execvp(command[0], command);   /* execute a process */

    // Because exec replaces the current process image with the new program,
    // this line will only ever be reached if exec fails for whatever reason.
    // Output the error information.
    perror("exec failed");
    return 2;

  // fork() returns the child process' process ID (PID) in the parent process.
  } else {
    // Wait for the child process to finish executing.
    waitpid(pid, NULL, 0);
  }

  return 0;
}
