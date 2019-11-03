#pragma once
#include <queue>
#include "abstract/event.h"
#include "abstract/thread.h"
#include "abstract/scheduling_decision.h"
#include "scheduler.h"

/**
 * A representation of a scheduling queue that uses first-come, first-served.
 */
class FcfsScheduler : public Scheduler {
public:
  virtual SchedulingDecision* get_next_thread(const Event* event) override;

  virtual void enqueue(const Event* event, Thread* thread) override;

private:
  std::queue<Thread*> threads;

};
