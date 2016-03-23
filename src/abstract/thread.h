#pragma once
#include "burst.h"
#include <queue>
#include <stddef.h>


// Forward declaration. This is needed if your Thread and Process classes keep
// pointers to each other, to break the circular dependency for the compiler.
struct Process;


/**
 * Represents a thread of execution, with one or more CPU + I/O bursts.
 */
struct Thread {
  /**
   * Represents valid states in which threads / processes can exist.
   */
  enum State {
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    EXIT
  };

  /**
   * The ID of this thread
   */
  int id;

  /**
   * The process associated with this thread.
   */
  Process* process;

  /**
   * All bursts that are a part of this thread.
   */
  std::queue<Burst*> bursts;

  /**
   * The current state of the thread.
   */
  State current_state = NEW;

  /**
   * The previous state of the thread.
   */
  State previous_state;

  /**
   * The time at which this thread arrived in the queue.
   */
  size_t arrival_time = -1;

  /**
   * The time at which this thread was first executed.
   */
  size_t start_time = -1;

  /**
   * The time at which this thread finished executing.
   */
  size_t end_time = -1;

  /**
   * The total amount of time spent executing on the CPU for this thread.
   */
  size_t service_time = 0;

  /**
   * The total amount of time spent doing I/O for this thread.
   */
  size_t io_time = 0;


  /**
   *  * Sets the thread to ready.
   *   */
  void set_ready(size_t time);

  /**
   *  * Sets the thread to running. Also records the start time if this is
   *  the
   *   * first time the thread has run.
   *    */
  void set_running(size_t time);

  /**
   *  * Sets the thread to blocked, waiting on IO.
   *   */
  void set_blocked(size_t time);

  /**
   *  * Sets the thread to finished.
   *   */
  void set_finished(size_t time);

  /**
   *  * Returns the response time for this thread.
   *   */
  size_t response_time() const;

  /**
   *  * Returns the turnaround time for this thread.
   *   */
  size_t turnaround_time() const;
};
