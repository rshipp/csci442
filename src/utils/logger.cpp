#include <iostream>
#include "logger.h"

using namespace std;

void Logger::print_state_transition(
      const Event* event,
      Thread::State before_state,
      Thread::State after_state) const {

  cout << "At time " << event->time << ":" << endl;
  cout << "    " << event_type_str(event->type) << endl;
  cout << "    Thread " << event->thread->id << " in process " << event->thread->process->pid << " [" << process_type_str(event->thread->process->type) << "]" << endl;
  if (event->scheduling_decision) {
    cout << "    " << event->scheduling_decision->explanation << endl;
  } else {
    cout << "    Transitioned from " << event->thread->StateString[before_state] << " to " << event->thread->StateString[after_state] << endl;
  }
  cout << endl;
}
