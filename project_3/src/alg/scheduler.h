#pragma once
#include "abstract/scheduling_decision.h"
#include "abstract/event.h"
#include "abstract/thread.h"

/**
 * Abstract base class (interface) for all scheduling algorithms.
 */
struct Scheduler {
  /**
   * Returns the next thread that should be executed, or NULL if there is no
   * thread available to run.
   */
  virtual SchedulingDecision* get_next_thread(const Event* event) = 0;

  /**
   * Enqueues the given thread in this scheduler's ready queues.
   */
  virtual void enqueue(const Event* event, Thread* thread) = 0;
};
