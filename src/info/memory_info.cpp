#include "memory_info.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;


MemoryInfo get_memory_info() {
  ifstream meminfo_file(PROC_ROOT "/meminfo");
  MemoryInfo memory_info;
  string line;

  if (!meminfo_file) {
    cerr << "Unable to read from /proc/meminfo" << endl;
    exit(EXIT_FAILURE);
  }

  while (getline(meminfo_file, line)) {
    if(line.find("MemTotal:") == 0) {
      memory_info.total_memory = int_from_line(line, "MemTotal:");
    } else if(line.find("MemFree:") == 0) {
      memory_info.free_memory = int_from_line(line, "MemFree:");
    } else if(line.find("Buffers:") == 0) {
      memory_info.buffers_memory = int_from_line(line, "Buffers:");
    } else if(line.find("Cached:") == 0) {
      memory_info.cached_memory = int_from_line(line, "Cached:");
    } else if(line.find("SwapTotal:") == 0) {
      memory_info.total_swap = int_from_line(line, "SwapTotal:");
    } else if(line.find("SwapFree:") == 0) {
      memory_info.free_swap = int_from_line(line, "SwapFree:");
    }
  }

  return memory_info;
}

int int_from_line(string line, const char* label) {
   return atoi(line.substr(string(label).size(), line.size()).c_str());
}
