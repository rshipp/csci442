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
