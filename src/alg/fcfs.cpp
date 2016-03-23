#include "abstract/scheduling_decision.h"
#include "abstract/event.h"
#include "abstract/thread.h"
#include "fcfs.h"
#include <stddef.h>

using namespace std;


SchedulingDecision* FcfsScheduler::get_next_thread(const Event* event) {
    SchedulingDecision* sd = new SchedulingDecision();
    sd->thread = threads.front();
    threads.pop();
    return sd;
}

void FcfsScheduler::enqueue(const Event* event, Thread* thread) {
    threads.push(thread);
}
