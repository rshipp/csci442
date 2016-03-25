#include "simulation.h"
#include "abstract/scheduling_decision.h"

using namespace std;

void Simulation::run(priority_queue<Event*, vector<const Event*>, EventComparator> events_queue) {

  events = events_queue;
  // While their are still events to process, invoke the corresponding methods
  // to handle them.
  while (!events.empty()) {
    const Event* event = events.top();
    events.pop();

    // Invoke the appropriate method for the given event type.
    switch (event->type) {
      case Event::THREAD_ARRIVED:
        handle_thread_arrived(event);
        break;
      case Event::THREAD_DISPATCH_COMPLETED:
        handle_thread_dispatch_completed(event);
        break;
      case Event::PROCESS_DISPATCH_COMPLETED:
        handle_process_dispatch_completed(event);
        break;
      case Event::CPU_BURST_COMPLETED:
        handle_cpu_burst_completed(event);
        break;
      case Event::IO_BURST_COMPLETED:
        handle_io_burst_completed(event);
        break;
      case Event::THREAD_COMPLETED:
        handle_thread_completed(event);
        break;
      case Event::THREAD_PREEMPTED:
        handle_thread_preempted(event);
        break;
      case Event::DISPATCHER_INVOKED:
        handle_dispatcher_invoked(event);
        break;
    }
  }
}

void Simulation::handle_thread_arrived(const Event* event) {
  // Corresponds to 'Set ready' in flow chart
  event->thread->set_ready(event->time);

  // Corresponds to 'Enqueue' in flow chart
  scheduler->enqueue(event, event->thread);

  // If the processor is currently idle, dispatch the thread.
  if (!active_thread) {
    // Add a new event to the queue; subsequent logic in the flow chart would then be
    // implemented in a different method (e.g. handle_dispatcher_invoked).
    events.push(new Event(Event::DISPATCHER_INVOKED, event->time, active_thread));
  }

  logger.print_state_transition(event, Thread::NEW, Thread::READY);
}

void Simulation::handle_dispatcher_invoked(const Event* event) {
  // Get scheduling decision
  SchedulingDecision* sd = scheduler->get_next_thread(event);
  Thread* last_thread;
  if (active_thread) {
      last_thread = active_thread;
  }
  // Set current thread
  active_thread = sd->thread;

  // If we made a decision, do a process or thread switch
  if (active_thread && last_thread) {
    if (active_thread->process == last_thread->process) {
      events.push(new Event(Event::THREAD_DISPATCH_COMPLETED, event->time, active_thread));
    } else {
      events.push(new Event(Event::PROCESS_DISPATCH_COMPLETED, event->time, active_thread));
    }
  } else if (active_thread) {
    events.push(new Event(Event::PROCESS_DISPATCH_COMPLETED, event->time, active_thread));
  }
}

void Simulation::handle_thread_dispatch_completed(const Event* event) {
  // Set running
  active_thread->set_running(event->time);

  SchedulingDecision* sd = scheduler->get_next_thread(event);
  if (sd->time_slice < active_thread->bursts.front()->length) {
    events.push(new Event(Event::THREAD_PREEMPTED, event->time, active_thread));
  } else {
    events.push(new Event(Event::CPU_BURST_COMPLETED, event->time, active_thread));
  }
}

void Simulation::handle_process_dispatch_completed(const Event* event) {
  handle_thread_dispatch_completed(event);
}

void Simulation::handle_cpu_burst_completed(const Event* event) {
  // Pop burst
  active_thread->bursts.pop();

  if (active_thread->bursts.empty()) {
    // Last burst
    events.push(new Event(Event::THREAD_COMPLETED, event->time, active_thread));
  } else {
    events.push(new Event(Event::IO_BURST_COMPLETED, event->time, active_thread));
  }

  // Unset current thread
  active_thread = nullptr;
  events.push(new Event(Event::DISPATCHER_INVOKED, event->time, active_thread));
}

void Simulation::handle_thread_preempted(const Event* event) {
  // Set ready
  
  // Enqueue

  // Decrease CPU burst
}

void Simulation::handle_io_burst_completed(const Event* event) {
  // Set ready
  event->thread->set_ready(event->time);
  // Enqueue
  scheduler->enqueue(event, event->thread);
  // Pop burst
  event->thread->bursts.pop();

  if (!active_thread) {
    events.push(new Event(Event::DISPATCHER_INVOKED, event->time, active_thread));
  }
}

void Simulation::handle_thread_completed(const Event* event) {
  event->thread->set_finished(event->time);
}
