#include "process_info.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

using namespace std;


ProcessInfo get_process(int pid, const char* basedir) {
  // get the pid in a string
  stringstream ss;
  ss << pid;
  string pid_str = ss.str();

  ProcessInfo process_info;

  // open the first file
  string stat_filename = string(basedir) + string("/") + pid_str + string("/stat");
  ifstream stat_file(stat_filename.c_str());
  if (!stat_file) {
    cerr << "Unable to read from " << basedir << "/" << pid_str << "/stat" << endl;
    exit(EXIT_FAILURE);
  }

  stat_file
      >> process_info.pid
      >> process_info.comm
      >> process_info.state
      >> process_info.ppid
      >> process_info.pgrp
      >> process_info.session
      >> process_info.tty_nr
      >> process_info.tpgid
      >> process_info.flags
      >> process_info.minflt
      >> process_info.cminflt
      >> process_info.majflt
      >> process_info.cmajflt
      >> process_info.utime
      >> process_info.stime
      >> process_info.cutime
      >> process_info.cstime
      >> process_info.priority
      >> process_info.nice
      >> process_info.num_threads
      >> process_info.itrealvalue
      >> process_info.starttime
      >> process_info.vsize
      >> process_info.rss
      >> process_info.rsslim
      >> process_info.startcode
      >> process_info.endcode
      >> process_info.startstack
      >> process_info.kstkesp
      >> process_info.kstkeip
      >> process_info.signal
      >> process_info.blocked
      >> process_info.sigignore
      >> process_info.sigcatch
      >> process_info.wchan
      >> process_info.nswap
      >> process_info.cnswap
      >> process_info.exit_signal
      >> process_info.processor
      >> process_info.rt_priority
      >> process_info.policy
      >> process_info.delayacct_blkio_ticks
      >> process_info.guest_time
      >> process_info.cguest_time;

  // open the second file
  string statm_filename = string(basedir) + string("/") + pid_str + string("/statm");
  ifstream statm_file(statm_filename.c_str());
  if (!statm_file) {
    cerr << "Unable to read from " << basedir << "/" << pid_str << "/statm" << endl;
    exit(EXIT_FAILURE);
  }

  statm_file
      >> process_info.size
      >> process_info.resident
      >> process_info.share
      >> process_info.trs
      >> process_info.lrs
      >> process_info.drs
      >> process_info.dt;

  // open the third file
  string cmdline_filename = string(basedir) + string("/") + pid_str + string("/cmdline");
  ifstream cmdline_file(cmdline_filename.c_str());
  if (!cmdline_file) {
    cerr << "Unable to read from " << basedir << "/" << pid_str << "/cmdline" << endl;
    exit(EXIT_FAILURE);
  }

  cmdline_file
      >> process_info.command_line;
  // fix string format
  if(process_info.command_line.size() > 0) {
    replace(process_info.command_line.begin(), process_info.command_line.end(), '\0', ' ');
    process_info.command_line.erase(process_info.command_line.size()-1, 1);
  } else {
    // default to command
    process_info.command_line = string(process_info.comm);
    process_info.command_line.erase(process_info.command_line.find('('), 1);
    process_info.command_line.erase(process_info.command_line.find(')'), 1);
  }

  // get info for threads
  string task_dir = string(basedir) + string("/") + pid_str + string("/task");
  process_info.threads = get_all_processes(task_dir.c_str());

  // Iterate over each thread and set its tgid to the parent's pid
  for (size_t i = 0; i < process_info.threads.size(); i++) {
    process_info.threads[i].tgid = process_info.pid;
  }



  return process_info;
}


vector<ProcessInfo> get_all_processes(const char* basedir) {

  DIR* dir = opendir(basedir);
  if(!dir) {
    return vector<ProcessInfo>();
  }
  struct dirent * entry;

  vector<ProcessInfo> processes;

  while ((entry = readdir(dir)) != NULL) {
    int pid = atoi(entry->d_name);

    // atoi couldn't convert d_name to an int, so skip this entry.
    if (pid <= 0) {
      continue;
    }

    // Process the PID-like entry.
    ProcessInfo process = get_process(pid, basedir);
    processes.insert(processes.end(), process);
  }

  closedir(dir);

  return processes;
}
