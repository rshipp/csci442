#include "simulation.h"

using namespace std;

void Simulation::run(priority_queue<Event*, vector<const Event*>, EventComparator> events_queue) {

  events = events_queue;
  // While their are still events to process, invoke the corresponding methods
  // to handle them.
  while (!events.empty()) {
    const Event* event = events.top();
    events.pop();

    // Invoke the appropriate method for the given event type.
    switch (event->type) {
      case Event::THREAD_ARRIVED:
        handle_thread_arrived(event);
        break;
      case Event::THREAD_DISPATCH_COMPLETED:
        handle_thread_dispatch_completed(event);
        break;
      case Event::PROCESS_DISPATCH_COMPLETED:
        handle_process_dispatch_completed(event);
        break;
      case Event::CPU_BURST_COMPLETED:
        handle_cpu_burst_completed(event);
        break;
      case Event::IO_BURST_COMPLETED:
        handle_io_burst_completed(event);
        break;
      case Event::THREAD_COMPLETED:
        handle_thread_completed(event);
        break;
      case Event::THREAD_PREEMPTED:
        handle_thread_preempted(event);
        break;
      case Event::DISPATCHER_INVOKED:
        handle_dispatcher_invoked(event);
        break;
    }
  }
}

void Simulation::handle_thread_arrived(const Event* event) {
  // Corresponds to 'Set ready' in flow chart
  event->thread->set_ready(event->time);

  // Corresponds to 'Enqueue' in flow chart
  scheduler->enqueue(event, event->thread);

  // If the processor is currently idle, dispatch the thread.
  if (!active_thread) {
    // Add a new event to the queue; subsequent logic in the flow chart would then be
    // implemented in a different method (e.g. handle_dispatcher_invoked).
    events.push(new Event(Event::DISPATCHER_INVOKED, event->time, nullptr));
  }
}

void Simulation::handle_dispatcher_invoked(const Event* event) {
}
void Simulation::handle_thread_dispatch_completed(const Event* event) {
}
void Simulation::handle_process_dispatch_completed(const Event* event) {
}
void Simulation::handle_cpu_burst_completed(const Event* event) {
}
void Simulation::handle_thread_preempted(const Event* event) {
}
void Simulation::handle_io_burst_completed(const Event* event) {
}
void Simulation::handle_thread_completed(const Event* event) {
}
