#include "system_info.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

using namespace std;


double get_uptime() {
  ifstream uptime_file(PROC_ROOT "/uptime");

  if (!uptime_file) {
    cerr << "Unable to read from /proc/uptime" << endl;
    exit(EXIT_FAILURE);
  }

  double uptime;
  uptime_file
      >> uptime;

  return uptime;
}


SystemInfo get_system_info() {
  SystemInfo system_info;

  // Found elsewhere
  system_info.uptime = get_uptime();
  system_info.load_average = get_load_average();
  system_info.memory_info = get_memory_info();
  system_info.cpus = get_cpu_info();
  system_info.processes = get_all_processes(PROC_ROOT);

  // Computed here
  system_info.num_kernel_threads = 0;
  system_info.num_user_threads = 0;
  system_info.num_running = 0;

  // for each process P returned by get_all_processes(PROC_ROOT)
  vector<ProcessInfo>::iterator it;
  for(it = system_info.processes.begin(); it != system_info.processes.end(); it++) {
    ProcessInfo process = *it;
    // if is_kernel_thread
    if(process.is_kernel_thread()) {
      system_info.num_kernel_threads++;
    }

    // if is_running
    if(process.state == 'R') {
      system_info.num_running++;
    }

    // for each thread in P (the current process)
    vector<ProcessInfo>::iterator t_it;
    for(t_it = process.threads.begin(); t_it != process.threads.end(); t_it++) {
      // if is_user_thread
      if(t_it->is_user_thread()) {
        system_info.num_user_threads++;
      }
    }
  }

  // num_threads = num_kernel_threads + num_user_threads
  system_info.num_threads = system_info.num_user_threads + system_info.num_kernel_threads;

  // total tasks
  system_info.num_processes = system_info.processes.size();


  return system_info;
}
