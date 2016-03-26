#include <iostream>
#include <iomanip>
#include "logger.h"

using namespace std;

void Logger::print_state_transition(
      const Event* event,
      Thread::State before_state,
      Thread::State after_state) const {

  if (verbose) {
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
}

void Logger::print_process_details(Process* process) const {
  if (per_thread) {
    cout << "Process " << process->pid << " [" << process_type_str(process->type) << "]:" << endl;
    vector<Thread*>::iterator t_it;
    for(t_it = process->threads.begin(); t_it != process->threads.end(); t_it++) {
      cout << "    Thread " << (*t_it)->id << ":  " <<
              "ARR: " << setw(7) << left << (*t_it)->arrival_time <<
              "CPU: " << setw(7) << left << (*t_it)->service_time <<
              "I/O: " << setw(7) << left << (*t_it)->io_time <<
              "TRT: " << setw(7) << left << (*t_it)->turnaround_time() <<
              "END: " << setw(7) << left << (*t_it)->end_time <<
          endl;
    }
    cout << endl;
  }
}
