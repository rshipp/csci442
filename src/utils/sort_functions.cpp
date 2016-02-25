#include "sort_functions.h"
#include <algorithm>

using namespace std;

bool pidSort(const ProcessInfo& a, const ProcessInfo& b) {
  return a.pid < b.pid;
}

bool cpuSort(const ProcessInfo& a, const ProcessInfo& b) {
  return a.cpu_percent > b.cpu_percent;
}

bool memSort(const ProcessInfo& a, const ProcessInfo& b) {
  return a.rss > b.rss;
}

bool timeSort(const ProcessInfo& a, const ProcessInfo& b) {
  return (a.utime+a.stime) > (b.utime+b.stime);
}

void sortProcesses(vector<ProcessInfo>& processes, string sortby) {
  if(sortby == "PID") {
    sort(processes.begin(), processes.end(), pidSort);
  } else if(sortby == "CPU") {
    sort(processes.begin(), processes.end(), cpuSort);
  } else if(sortby == "MEM") {
    sort(processes.begin(), processes.end(), memSort);
  } else if(sortby == "TIME") {
    sort(processes.begin(), processes.end(), timeSort);
  }
}
