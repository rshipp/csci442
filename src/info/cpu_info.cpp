#include "cpu_info.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;


vector<CpuInfo> get_cpu_info() {
  ifstream stat_file(PROC_ROOT "/stat");
  vector<CpuInfo> cpu_info;
  string line;

  if (!stat_file) {
    cerr << "Unable to read from /proc/stat" << endl;
    exit(EXIT_FAILURE);
  }

  while (getline(stat_file, line)) {
    if(line.find("cpu") == 0) {
      CpuInfo cpu;
      // Strip off the first "cpuX" characters and make a stream.
      istringstream iss(line.substr(4, line.size()));
      iss
          >> cpu.user_time
          >> cpu.nice_time
          >> cpu.system_time
          >> cpu.idle_time
          >> cpu.io_wait_time
          >> cpu.irq_time
          >> cpu.softirq_time
          >> cpu.steal_time
          >> cpu.guest_time
          >> cpu.guest_nice_time;
      cpu_info.insert(cpu_info.end(), cpu);
    }
  }

  return cpu_info;
}


CpuInfo operator -(const CpuInfo& lhs, const CpuInfo& rhs) {
  // TODO: implement me
  return CpuInfo();
}
