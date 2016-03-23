#include "simulation.h"

void Simulation::run(const std::string& filename) {
  // While their are still events to process, invoke the corresponding methods
  // to handle them.
  while (!events.empty()) {
    const Event* event = events.top();
    events.pop();

    // Invoke the appropriate method for the given event type.
    switch (event->type) {
      case Event::THREAD_ARRIVED:
        //handle_thread_arrived(event);
        break;

      // Similar cases for the other event types.
    }
  }
}

void Simulation::handle_thread_arrived(const Event* event) {
  // Corresponds to 'Set ready' in flow chart
  //event->thread->set_ready(event->time);

  // Corresponds to 'Enqueue' in flow chart
  scheduler->enqueue(event, event->thread);

  /*
  // If the processor is currently idle, dispatch the thread.
  if (!active_thread) {
    // Add a new event to the queue; subsequent logic in the flow chart would then be
    // implemented in a different method (e.g. handle_dispatcher_invoked).
    event_queue.push(new Event(Event::DISPATCHER_INVOKED, event->time, nullptr));
  }
  */
}
