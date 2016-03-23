#pragma once
#include <vector>
#include <string>
#include "abstract/process.h"

struct Config {
  int thread_switch_overhead;

  int process_switch_overhead;

  std::vector<Process*> processes;
};

Config parse_config(std::string in_file);
