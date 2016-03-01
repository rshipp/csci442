#pragma once
#include "thread.h"
#include <string>


/**
 * Representation of a single scheduler decision.
 */
struct SchedulingDecision {
  /**
   * The next thread to run, or NULL if there are no threads in the ready state.
   */
  Thread* thread = nullptr;

  /**
   * The amount of time after which the thread should be preempted, or -1 
   * (MAX_LONG == infinite time slice) if the thread should not be preempted.
   */
  size_t time_slice = -1;

  /**
   * A brief message concerning this scheduling choice.
   */
  std::string explanation;
};
