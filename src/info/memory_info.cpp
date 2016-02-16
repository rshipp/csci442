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
      memory_info.total_memory = atoi(line.substr(string("MemTotal:").size(), line.size()).c_str());
    } else if(line.find("MemFree:") == 0) {
      memory_info.free_memory = atoi(line.substr(string("MemFree:").size(), line.size()).c_str());
    } else if(line.find("Buffers:") == 0) {
      memory_info.buffers_memory = atoi(line.substr(string("Buffers:").size(), line.size()).c_str());
    } else if(line.find("Cached:") == 0) {
      memory_info.cached_memory = atoi(line.substr(string("Cached:").size(), line.size()).c_str());
    } else if(line.find("SwapTotal:") == 0) {
      memory_info.total_swap = atoi(line.substr(string("SwapTotal:").size(), line.size()).c_str());
    } else if(line.find("SwapFree:") == 0) {
      memory_info.free_swap = atoi(line.substr(string("SwapFree:").size(), line.size()).c_str());
    }
  }

  return memory_info;
}
