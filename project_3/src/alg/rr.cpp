#include "abstract/scheduling_decision.h"
#include "abstract/event.h"
#include "abstract/thread.h"
#include "rr.h"
#include <stddef.h>

using namespace std;

SchedulingDecision* RoundRobinScheduler::get_next_thread(const Event* event) {
  SchedulingDecision* sd = new SchedulingDecision();
  sd->time_slice = 3;
  if (threads.empty()) {
    return sd;
  }
  sd->thread = threads.front();
  sd->explanation = string("Selected from ") + to_string(threads.size()) + string(" threads; will run to completion of burst, or preemption");
  threads.pop();
  return sd;
}

void RoundRobinScheduler::enqueue(const Event* event, Thread* thread) {
  threads.push(thread);
}
