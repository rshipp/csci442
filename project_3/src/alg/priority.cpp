#include "abstract/scheduling_decision.h"
#include "abstract/event.h"
#include "abstract/thread.h"
#include "abstract/process.h"
#include "priority.h"
#include <stddef.h>

#include <iostream>

using namespace std;


SchedulingDecision* PriorityScheduler::get_next_thread(const Event* event) {
  SchedulingDecision* sd = new SchedulingDecision();
  if (!system.empty()) {
    sd->thread = system.front();
    sd->explanation = "Selected from " + to_string(system.size()) + " threads in queue; will run to completion of burst";
    system.pop();
  } else if (!interactive.empty()) {
    sd->thread = interactive.front();
    sd->explanation = "Selected from " + to_string(interactive.size()) + " threads in queue; will run to completion of burst";
    interactive.pop();
  } else if (!normal.empty()) {
    sd->thread = normal.front();
    sd->explanation = "Selected from " + to_string(normal.size()) + " threads in queue; will run to completion of burst";
    normal.pop();
  } else if (!batch.empty()) {
    sd->thread = batch.front();
    sd->explanation = "Selected from " + to_string(batch.size()) + " threads in queue; will run to completion of burst";
    batch.pop();
  }
  return sd;
}

void PriorityScheduler::enqueue(const Event* event, Thread* thread) {
  switch(thread->process->type) {
    case SYSTEM:
      system.push(thread);
      break;
    case INTERACTIVE:
      interactive.push(thread);
      break;
    case NORMAL:
      normal.push(thread);
      break;
    case BATCH:
      batch.push(thread);
      break;
  }
}
