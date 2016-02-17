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
  CpuInfo delta;
  delta.user_time = lhs.user_time - rhs.user_time;
  delta.nice_time = lhs.nice_time - rhs.nice_time;
  delta.system_time = lhs.system_time - rhs.system_time;
  delta.idle_time = lhs.idle_time - rhs.idle_time;
  delta.io_wait_time = lhs.io_wait_time - rhs.io_wait_time;
  delta.irq_time = lhs.irq_time - rhs.irq_time;
  delta.softirq_time = lhs.softirq_time - rhs.softirq_time;
  delta.steal_time = lhs.steal_time - rhs.steal_time;
  delta.guest_time = lhs.guest_time - rhs.guest_time;
  delta.guest_nice_time = lhs.guest_nice_time - rhs.guest_nice_time;
  return delta;
}
