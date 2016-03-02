#include "config.h"
#include "abstract/burst.h"
#include "abstract/process.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;


Config parse_config(string in_file) {
  ifstream fs(in_file);
  Config config;
  string line;

  if (!fs) {
    cerr << "Unable to read from " << in_file << endl;
    exit(EXIT_FAILURE);
  }

  int num_processes;
  fs >> num_processes
     >> config.thread_switch_overhead
     >> config.process_switch_overhead;

  for(int i=0; i<num_processes; i++) {
    Process *process = new Process();
    
    int num_threads, process_type;
    fs >> process->pid
       >> process_type
       >> num_threads;
    switch(process_type) {
        case SYSTEM:
            process->type = SYSTEM;
            break;
        case INTERACTIVE:
            process->type = INTERACTIVE;
            break;
        case NORMAL:
            process->type = NORMAL;
            break;
        case BATCH:
            process->type = BATCH;
            break;
    }

    for(int j=0; j<num_threads; j++) {
      Thread *thread = new Thread();
      thread->process = process;
      thread->id = j;

      int num_cpu_bursts;
      fs >> thread->arrival_time
         >> num_cpu_bursts;

      bool cpuType = true;
      for(int k=0; k<num_cpu_bursts*2-1; k++) {
        // read n*2-1 bursts from n lines
        BurstType burst_type;
        if(cpuType) {
          burst_type = CPU;
        } else {
          burst_type = IO;
        }
        cpuType = !cpuType;

        int burst_length;
        fs >> burst_length;

        Burst *burst = new Burst(burst_type, burst_length);

        thread->bursts.push(burst);
      }

      process->threads.insert(process->threads.end(), thread);
    }

    config.processes.insert(config.processes.end(), process);
  }

  return config;
}

