#pragma once
#include "scheduling_decision.h"
#include "thread.h"

/**
 * Represents the various types of events that can happen in the simulation.
 */
struct Event {
  /**
   * The type of the event.
   */
  enum Type {
    /**
     * A thread was created in the system.
     */
    THREAD_ARRIVED,

    /**
     * A thread has finished dispatching.
     */
    THREAD_DISPATCH_COMPLETED,

    /**
     * A process has finished dispatching.
     */
    PROCESS_DISPATCH_COMPLETED,

    /**
     * A CPU burst has finished.
     */
    CPU_BURST_COMPLETED,

    /**
     * An IO burst has finished.
     */
    IO_BURST_COMPLETED,

    /**
     * Like CPU_BURST_COMPLETED, but also represents that the thread has
     * completely finished.
     */
    THREAD_COMPLETED,

    /**
     * A thread has been preempted before its time slice expired.
     */
    THREAD_PREEMPTED,

    /**
     * The OS needs to schedule a new thread to run, if one is available.
     */
    DISPATCHER_INVOKED
  };

  /**
   * The type of event.
   */
  Type type;

  /**
   * The time at which the event occurs.
   */
  int time;

  /**
   * The thread for which the event applies (may be null).
   */
  Thread* thread;

  /**
   * The associated scheduling decision, if any.
   */
  const SchedulingDecision* scheduling_decision;
};
