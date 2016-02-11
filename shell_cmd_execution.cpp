/**
 * This file contains implementations of the functions that are responsible for
 * executing commands that are not builtins.
 *
 * Much of the code you write will probably be in this file. Add helper methods
 * as needed to keep your code clean. You'll lose points if you write a single
 * monolithic function!
 */

#include "shell.h"
#include "shell_partitioning.h"
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>

using namespace std;


int Shell::execute_external_command(vector<string>& tokens) {
  int pid, status, pipe_fd[2], in_pipe_fd;
  vector<command_t> commands;  

  // partition commands
  if(!partition_tokens(tokens, commands)) {
    return 1;
  }

  vector<command_t>::iterator c_it;
  for(c_it = commands.begin(); c_it != commands.end(); c_it++) {

    if((*c_it).output_type == WRITE_TO_PIPE) {
      // create the pipe
      if(pipe(pipe_fd) < 0) {
        perror("pipe");
        return 1;
      }
    }

    // Attempt to fork off a new process for each command.
    if ((pid = fork()) == -1) {
      perror("fork failed");
      return 1;
    }

    // fork() returns 0 in the child process
    if(pid == 0) {
      // set up STDIN
      if((*c_it).input_type == READ_FROM_PIPE) {
        if(dup2(in_pipe_fd, STDIN_FILENO) == -1) {
          perror("dup2");
          close(pipe_fd[0]);
          exit(1);
        }
      } else if((*c_it).input_type == READ_FROM_FILE) {
        int fd = open((*c_it).infile.c_str(), O_RDONLY);
        if(fd == -1) {
          perror("cannot open file for reading");
          exit(errno);
        }
        if(dup2(fd, STDIN_FILENO) == -1) {
          perror("dup2");
          close(fd);
          exit(1);
        }
      }

      // set up STDOUT
      if((*c_it).output_type == WRITE_TO_PIPE) {
        if(dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
          perror("dup2");
          exit(errno);
        }
      } else if((*c_it).output_type == WRITE_TO_FILE) {
        int fd = open((*c_it).outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(fd == -1) {
          perror("cannot open file for writing");
          exit(errno);
        }
        if(dup2(fd, STDOUT_FILENO) == -1) {
          perror("dup2");
          exit(errno);
        }
      } else if((*c_it).output_type == APPEND_TO_FILE) {
        int fd = open((*c_it).outfile.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if(fd == -1) {
          perror("cannot open file for writing");
          exit(errno);
        }
        if(dup2(fd, STDOUT_FILENO) == -1) {
          perror("dup2");
          exit(errno);
        }
      }


      // Execute the given command.
      char* c_command[(*c_it).argv.size()];
      int i = 0;
      vector<string>::iterator it;
      for(it = (*c_it).argv.begin(); it != (*c_it).argv.end(); it++) {
        c_command[i++] = (char*)(*it).c_str();
      }
      c_command[i] = (char*)0;
      execvp(c_command[0], c_command);   /* execute a process */

      // Because exec replaces the current process image with the new program,
      // this line will only ever be reached if exec fails for whatever reason.
      // Output the error information.
      perror("exec failed");
      exit(2);

    // fork() returns the child process' process ID (PID) in the parent process.
    } else {
      // hook up the pipe
      if((*c_it).input_type == READ_FROM_PIPE) {
        close(in_pipe_fd);
      }
      if((*c_it).output_type == WRITE_TO_PIPE) {
        in_pipe_fd = pipe_fd[0];
        close(pipe_fd[1]);
      }
      // Wait for the child process to finish executing.
      waitpid(pid, &status, 0);
    }
  }


  return WEXITSTATUS(status);
}
