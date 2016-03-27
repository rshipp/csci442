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

void Logger::print_statistics(SystemStats stats) const {
  cout << setw(28) << left << "SYSTEM THREADS:" << "\n" <<
          setw(28) << left << "    Total count:" << setw(6) << right << stats.system_thread_count() << "\n" <<
          setw(28) << left << "    Avg response time:" << setw(6) << right << fixed << setprecision(2) << stats.system_response_time() << "\n" <<
          setw(28) << left << "    Avg turnaround time:" << setw(6) << right << stats.system_turnaround_time() << "\n" <<
          "\n" <<
          setw(28) << left << "INTERACTIVE THREADS:" << "\n" <<
          setw(28) << left << "    Total count:" << setw(6) << right << stats.interactive_thread_count() << "\n" <<
          setw(28) << left << "    Avg response time:" << setw(6) << right << stats.interactive_response_time() << "\n" <<
          setw(28) << left << "    Avg turnaround time:" << setw(6) << right << stats.interactive_turnaround_time() << "\n" <<
          "\n" <<
          setw(28) << left << "NORMAL THREADS:" << "\n" <<
          setw(28) << left << "    Total count:"  << setw(6) << right << stats.normal_thread_count() << "\n" <<
          setw(28) << left << "    Avg response time:" << setw(6) << right << stats.normal_response_time() << "\n" <<
          setw(28) << left << "    Avg turnaround time:" << setw(6) << right << stats.normal_turnaround_time() << "\n" <<
          "\n" <<
          setw(28) << left << "BATCH THREADS:" << "\n" <<
          setw(28) << left << "    Total count:" << setw(6) << right << stats.batch_thread_count() << "\n" <<
          setw(28) << left << "    Avg response time:" << setw(6) << right << stats.batch_response_time() << "\n" <<
          setw(28) << left << "    Avg turnaround time:" << setw(6) << right << stats.batch_turnaround_time() << "\n" <<
          "\n" <<
          setw(28) << left << "Total elapsed time:" << setw(6) << right << stats.total_time() << "\n" <<
          setw(28) << left << "Total service time:" << setw(6) << right << stats.service_time() << "\n" <<
          setw(28) << left << "Total I/O time:" << setw(6) << right << stats.io_time() << "\n" <<
          setw(28) << left << "Total dispatch time:" << setw(6) << right << stats.dispatch_time() << "\n" <<
          setw(28) << left << "Total idle time:" << setw(6) << right << stats.idle_time() << "\n" <<
          "\n" <<
          setw(28) << left << "CPU utilization:" << setw(6) << right << stats.cpu_utilization() << "%" << "\n" <<
          setw(28) << left << "CPU efficiency:" << setw(6) << right << stats.cpu_efficiency() << "%" << "\n" <<
	      endl;
}
