#pragma once
#include <queue>
#include <vector>
#include "abstract/event.h"
#include "abstract/thread.h"
#include "abstract/scheduling_decision.h"
#include "scheduler.h"

/**
 * A representation of a scheduling queue that uses four queues
 */
class PriorityScheduler : public Scheduler {
public:
  virtual SchedulingDecision* get_next_thread(const Event* event) override;

  virtual void enqueue(const Event* event, Thread* thread) override;

private:
  std::queue<Thread*> system;
  std::queue<Thread*> interactive;
  std::queue<Thread*> normal;
  std::queue<Thread*> batch;

};
