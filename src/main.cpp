#include <iostream>
#include <queue>
#include "utils/flags.h"
#include "utils/config.h"
#include "utils/logger.h"
#include "abstract/event.h"
#include "abstract/process.h"
#include "abstract/scheduling_decision.h"
#include "alg/simulation.h"
#include "alg/scheduler.h"
#include "alg/fcfs.h"
#include "alg/rr.h"
#include "alg/priority.h"
#include "alg/custom.h"

using namespace std;


int main(int argc, char** argv) {
  Options options = parse_args(argc, argv);
  Config config = parse_config(options.in_file);

  // Add events to the queue
  priority_queue<Event*, vector<const Event*>, EventComparator> events;

  vector<Process*>::iterator p_it;
  for(p_it = config.processes.begin(); p_it != config.processes.end(); p_it++) {
    vector<Thread*>::iterator t_it;
    for(t_it = (*p_it)->threads.begin(); t_it != (*p_it)->threads.end(); t_it++) {
      events.push(new Event(Event::THREAD_ARRIVED, (*t_it)->arrival_time, *t_it));
    }
  }

  // Set up simulation
  Scheduler* scheduler;
  if (options.algorithm == "RR") {
    scheduler = new RoundRobinScheduler();
  } else if (options.algorithm == "PRIORITY") {
    scheduler = new PriorityScheduler();
  } else if (options.algorithm == "CUSTOM") {
    scheduler = new CustomScheduler();
  } else {
    // Default to fcfs
    scheduler = new FcfsScheduler();
  }
  Logger* logger = new Logger(options.verbose, options.per_thread);
  Simulation* simulation = new Simulation(scheduler, *logger, config.thread_switch_overhead, config.process_switch_overhead);

  // Run
  simulation->run(events);

  return 0;
}
