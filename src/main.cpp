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

  Scheduler* scheduler = new FcfsScheduler();
  Logger* logger = new Logger(false, false);
  Simulation* simulation = new Simulation(scheduler, *logger);
  simulation->run(events);

  // Print the queue
  // NOTE: The queue is destroyed at this time, so do this last.
  while (!events.empty()) {
    // Get the next event (the one that happens next) from the 'top' of the queue.
    const Event* event = events.top();

    // The top() method just returns the element, it doesn't remove it. You have
    // to call pop() to do that.
    events.pop();

    // Process the event using the appropriate logic. I use a switch statement
    // based on the type of the event. When handling an event, you might add
    // additional events to the queue. For example, if you encounter a
    // THREAD_COMPLETED event, you might add a new one for DISPATCHER_INVOKED.
    
    cout << "At time " << event->time << ":" << endl;
    cout << "    " << event_type_str(event->type) << endl;
    cout << "    Thread " << event->thread->id << " in process " << event->thread->process->pid << " [" << process_type_str(event->thread->process->type) << "]" << endl;
    cout << "    Transitioned from NEW to READY" << endl;
    cout << endl;
  }
  return 0;
}
