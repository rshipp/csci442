#pragma once
#include "scheduler.h"
#include "utils/logger.h"
#include <queue>

class Simulation {
// PUBLIC API METHODS
public:

  Simulation(Scheduler* scheduler, Logger logger)
      : scheduler(scheduler), logger(logger) {}

  void run(const std::string& filename);

// EVENT HANDLING METHODS
private:
  void handle_thread_arrived(const Event* event);


// UTILITY METHODS
private:


// CLASS INSTANCE VARIABLES
private:
    Scheduler* scheduler;
    Logger logger;
    std::priority_queue<Event*> events;
};
