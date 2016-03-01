#pragma once
#include "thread.h"
#include <vector>

/**
 * Represents a simple process, which encapsulates one or more threads.
 */
struct Process {
  /**
   * The type of the process (representing its relative priority).
   */
  enum Type {
    SYSTEM,
    INTERACTIVE,
    NORMAL,
    BATCH
  };

  /**
   * The ID of this process.
   */
  int pid;

  /**
   * The type of the process.
   */
  Type type;

  /**
   * All threads belonging to this process (as pointers).
   */
  std::vector<Thread*> threads;
};
