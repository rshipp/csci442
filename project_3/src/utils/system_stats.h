#pragma once
#include "abstract/process.h"
#include "config.h"

class SystemStats {
public:
  SystemStats(Config config) : config(config) {}

  int system_thread_count();
  float system_response_time();
  float system_turnaround_time();

  int interactive_thread_count();
  float interactive_response_time();
  float interactive_turnaround_time();

  int normal_thread_count();
  float normal_response_time();
  float normal_turnaround_time();

  int batch_thread_count();
  float batch_response_time();
  float batch_turnaround_time();

  int total_time();
  int service_time();
  int io_time();
  int dispatch_time();
  int idle_time();

  float cpu_utilization();
  float cpu_efficiency();

  size_t time = 0;
  size_t dispatch = 0;

private:
  Config config;

  int _thread_count(ProcessType type);
  float _response_time(ProcessType type);
  float _turnaround_time(ProcessType type);

};
