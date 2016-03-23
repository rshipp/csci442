#pragma once
#include "scheduler.h"
#include "utils/logger.h"
#include <queue>
#include <vector>

class Simulation {
// PUBLIC API METHODS
public:

  Simulation(Scheduler* scheduler, Logger logger)
      : scheduler(scheduler), logger(logger) {}

  void run(std::priority_queue<Event*, std::vector<const Event*>, EventComparator> events_queue);

// EVENT HANDLING METHODS
private:
  void handle_thread_arrived(const Event* event);
  void handle_dispatcher_invoked(const Event* event);
  void handle_thread_dispatch_completed(const Event* event);
  void handle_process_dispatch_completed(const Event* event);
  void handle_cpu_burst_completed(const Event* event);
  void handle_thread_preempted(const Event* event);
  void handle_io_burst_completed(const Event* event);
  void handle_thread_completed(const Event* event);

// UTILITY METHODS
private:


// CLASS INSTANCE VARIABLES
private:
    Thread* active_thread;
    Scheduler* scheduler;
    Logger logger;
    std::priority_queue<Event*, std::vector<const Event*>, EventComparator> events;
};
