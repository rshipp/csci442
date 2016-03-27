#pragma once
#include "config.h"
#include "system_stats.h"
#include "abstract/process.h"
#include "abstract/thread.h"
#include <vector>

using namespace std;

int SystemStats::_thread_count(ProcessType type) {
  int i = 0;
  vector<Process*>::iterator p_it;
  for(p_it = config.processes.begin(); p_it != config.processes.end(); p_it++) {
    if ((*p_it)->type == type) {
      i += (*p_it)->threads.size();
    }
  }
  return i;
}

float SystemStats::_response_time(ProcessType type) {
  int i = 0;
  int sum = 0;
  vector<Process*>::iterator p_it;
  for(p_it = config.processes.begin(); p_it != config.processes.end(); p_it++) {
    if ((*p_it)->type == type) {
      i += (*p_it)->threads.size();

      vector<Thread*>::iterator t_it;
      for(t_it = (*p_it)->threads.begin(); t_it != (*p_it)->threads.end(); t_it++) {
        sum += (*t_it)->response_time();
      }
    }
  }
  if (i == 0) {
    return 0.0F;
  }
  return (float)sum/(float)i;
}

float SystemStats::_turnaround_time(ProcessType type) {
  int i = 0;
  int sum = 0;
  vector<Process*>::iterator p_it;
  for(p_it = config.processes.begin(); p_it != config.processes.end(); p_it++) {
    if ((*p_it)->type == type) {
      i += (*p_it)->threads.size();

      vector<Thread*>::iterator t_it;
      for(t_it = (*p_it)->threads.begin(); t_it != (*p_it)->threads.end(); t_it++) {
        sum += (*t_it)->turnaround_time();
      }
    }
  }
  if (i == 0) {
    return 0.0F;
  }
  return (float)sum/(float)i;
}


int SystemStats::system_thread_count() {
  return _thread_count(SYSTEM);
}
float SystemStats::system_response_time() {
  return _response_time(SYSTEM);
}
float SystemStats::system_turnaround_time() {
  return _turnaround_time(SYSTEM);
}

int SystemStats::interactive_thread_count() {
  return _thread_count(INTERACTIVE);
}
float SystemStats::interactive_response_time() {
  return _response_time(INTERACTIVE);
}
float SystemStats::interactive_turnaround_time() {
  return _turnaround_time(INTERACTIVE);
}

int SystemStats::normal_thread_count() {
  return _thread_count(NORMAL);
}
float SystemStats::normal_response_time() {
  return _response_time(NORMAL);
}
float SystemStats::normal_turnaround_time() {
  return _turnaround_time(NORMAL);
}

int SystemStats::batch_thread_count() {
  return _thread_count(BATCH);
}
float SystemStats::batch_response_time() {
  return _response_time(BATCH);
}
float SystemStats::batch_turnaround_time() {
  return _turnaround_time(BATCH);
}

int SystemStats::total_time() {
  return time;
}
int SystemStats::service_time() {
  int sum = 0;
  vector<Process*>::iterator p_it;
  for(p_it = config.processes.begin(); p_it != config.processes.end(); p_it++) {
    vector<Thread*>::iterator t_it;
    for(t_it = (*p_it)->threads.begin(); t_it != (*p_it)->threads.end(); t_it++) {
      sum += (*t_it)->service_time;
    }
  }
  return sum;
}
int SystemStats::io_time() {
  int sum = 0;
  vector<Process*>::iterator p_it;
  for(p_it = config.processes.begin(); p_it != config.processes.end(); p_it++) {
    vector<Thread*>::iterator t_it;
    for(t_it = (*p_it)->threads.begin(); t_it != (*p_it)->threads.end(); t_it++) {
      sum += (*t_it)->io_time;
    }
  }
  return sum;
}
int SystemStats::dispatch_time() {
  return dispatch;
}
int SystemStats::idle_time() {
  return total_time() - (service_time() + dispatch_time());
}

float SystemStats::cpu_utilization() {
  return (float)(total_time()-idle_time())/(float)total_time()*100;
}
float SystemStats::cpu_efficiency() {
  return (float)service_time()/(float)total_time()*100;
}
