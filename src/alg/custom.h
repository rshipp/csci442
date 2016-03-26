#pragma once
#include <queue>
#include <map>
#include "abstract/event.h"
#include "abstract/thread.h"
#include "abstract/scheduling_decision.h"
#include "scheduler.h"

/**
 * A representation of a scheduling queue that uses round robin.
 */
class CustomScheduler : public Scheduler {
public:
  virtual SchedulingDecision* get_next_thread(const Event* event) override;

  virtual void enqueue(const Event* event, Thread* thread) override;

private:
  std::queue<Thread*> queue0;
  std::queue<Thread*> queue1;
  std::queue<Thread*> queue2;
  std::queue<Thread*> queue3;
  std::queue<Thread*> queue4;
  std::queue<Thread*> queue5;
  int max_queue = 5;

  std::map<Thread*, int> thread_map;

};
