#include "abstract/scheduling_decision.h"
#include "abstract/event.h"
#include "abstract/thread.h"
#include "custom.h"
#include <stddef.h>

using namespace std;

SchedulingDecision* CustomScheduler::get_next_thread(const Event* event) {
  SchedulingDecision* sd = new SchedulingDecision();
  sd->time_slice = 3;
  if (!queue0.empty()) {
    sd->thread = queue0.front();
    sd->explanation = "Selected from " + to_string(queue0.size()) + " threads in queue0; will run to completion of burst, or preemption";
    queue0.pop();
  } else if (!queue1.empty()) {
    sd->thread = queue1.front();
    sd->explanation = "Selected from " + to_string(queue1.size()) + " threads in queue1; will run to completion of burst, or preemption";
    queue1.pop();
  } else if (!queue2.empty()) {
    sd->thread = queue2.front();
    sd->explanation = "Selected from " + to_string(queue2.size()) + " threads in queue2; will run to completion of burst, or preemption";
    queue2.pop();
  } else if (!queue3.empty()) {
    sd->thread = queue3.front();
    sd->explanation = "Selected from " + to_string(queue3.size()) + " threads in queue3; will run to completion of burst, or preemption";
    queue3.pop();
  } else if (!queue4.empty()) {
    sd->thread = queue4.front();
    sd->explanation = "Selected from " + to_string(queue4.size()) + " threads in queue4; will run to completion of burst, or preemption";
    queue4.pop();
  } else if (!queue5.empty()) {
    sd->thread = queue5.front();
    sd->explanation = "Selected from " + to_string(queue5.size()) + " threads in queue5; will run to completion of burst, or preemption";
    queue5.pop();
  }
  return sd;
}

void CustomScheduler::enqueue(const Event* event, Thread* thread) {
  int queue;
  if (thread_map.count(thread)) {
    if (event->type == Event::THREAD_PREEMPTED) {
      if (thread_map[thread] < max_queue) {
        queue = thread_map[thread] + 1;
      } else {
        queue = 0;
      }
    } else {
      queue = thread_map[thread];
    }
  } else {
    queue = 0;
  }

  thread_map[thread] = queue;
  switch(queue) {
    case 0:
      queue0.push(thread);
      break;
    case 1:
      queue1.push(thread);
      break;
    case 2:
      queue2.push(thread);
      break;
    case 3:
      queue3.push(thread);
      break;
    case 4:
      queue4.push(thread);
      break;
    case 5:
      queue5.push(thread);
      break;
    }
}
