#include "config.h"
#include "burst.h"
#include "process.h"
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

  istringstream iss(line);
  iss >> config.num_processes
      >> config.thread_switch_overhead
      >> config.process_switch_overhead;

  //config.processes = vector<Process>();
  for(int i=0; i<config.num_processes; i++) {
    Process process;
    
    int num_threads, process_type;
    iss >> process.pid
        >> process_type
        >> num_threads;
    switch(process_type) {
        case SYSTEM:
            process.type = SYSTEM;
            break;
        case INTERACTIVE:
            process.type = INTERACTIVE;
            break;
        case NORMAL:
            process.type = NORMAL;
            break;
        case BATCH:
            process.type = BATCH;
            break;
    }

    //process.threads = vector<Thread>();
    for(int j=0; j<num_threads; j++) {
      Thread thread;
      thread.process = &process;

      int num_cpu_bursts;
      iss >> thread.arrival_time
          >> num_cpu_bursts;

      bool cpuType = true;
      //thread.bursts = queue<Burst>();
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
        iss >> burst_length;

        Burst *burst = new Burst(burst_type, burst_length);

        thread.bursts.push(burst);
      }

      process.threads.insert(process.threads.end(), &thread);
    }

    config.processes.insert(config.processes.end(), process);
  }

  return config;
}

