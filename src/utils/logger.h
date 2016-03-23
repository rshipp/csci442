#pragma once
#include "abstract/process.h"
#include "abstract/event.h"
#include "abstract/thread.h"

class Logger {
public:

  Logger(bool verbose, bool per_thread)
      : verbose(verbose), per_thread(per_thread) {}

  /**
   * If 'verbose' is set to true, outputs a human-readable message indicating
   * that the thread associated with the given event has transitioned from
   * before_state to after_state.
   */
  void print_state_transition(
      const Event* event,
      Thread::State before_state,
      Thread::State after_state) const;

  /**
   * Outputs a message if 'verbose' is true.
   */
  void print_verbose(
      const Event* event,
      Thread* thread,
      std::string message) const;

  /**
   * If 'per_thread' is set to true, outputs detailed information about a
   * process and its threads.
   */
  void print_process_details(Process* process) const;

  /**
   * Print overall statistics for the simulation.
   */
  //void print_statistics(SystemStats stats) const;

private:

  /**
   * Whether to display verbose output.
   */
  bool verbose;

  /**
   * Whether to display detailed per-process statistics.
   */
  bool per_thread;
};
