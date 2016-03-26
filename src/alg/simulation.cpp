#include "simulation.h"
#include "abstract/scheduling_decision.h"
#include <iostream>

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
    events.push(new Event(Event::DISPATCHER_INVOKED, event->time, event->thread));
  }

  logger.print_state_transition(event, event->thread->previous_state, event->thread->current_state);
}

void Simulation::handle_dispatcher_invoked(const Event* event) {
  // Get scheduling decision
  SchedulingDecision* sd = scheduler->get_next_thread(event);
  /*
  if (active_thread) {
    last_thread = active_thread;
  }*/
  // Set current thread
  active_thread = sd->thread;

  Event* new_event;
  // If we made a decision, do a process or thread switch
  if (active_thread && last_thread) {
    if (active_thread->process == last_thread->process) {
      new_event = new Event(Event::THREAD_DISPATCH_COMPLETED, event->time + thread_switch_overhead, active_thread);
    } else {
      new_event = new Event(Event::PROCESS_DISPATCH_COMPLETED, event->time + process_switch_overhead, active_thread);
    }
  } else if (active_thread) {
    new_event = new Event(Event::PROCESS_DISPATCH_COMPLETED, event->time + process_switch_overhead, active_thread);
  }

  if (active_thread) {
    //new_event->scheduling_decision = sd;
    events.push(new_event);
    Event* old_event = new Event(event->type, event->time, event->thread);
    old_event->scheduling_decision = sd;
    logger.print_state_transition(old_event, active_thread->current_state, active_thread->current_state);
  }
}

void Simulation::handle_dispatch_completed(const Event* event) {
  // Set running
  event->thread->set_running(event->time);

  if (time_slice != -1 && time_slice < event->thread->bursts.front()->length) {
    events.push(new Event(Event::THREAD_PREEMPTED, event->time + time_slice, event->thread));
  } else {
    events.push(new Event(Event::CPU_BURST_COMPLETED, event->time + event->thread->bursts.front()->length, event->thread));
  }

  // Set last thread = current thread
  last_thread = active_thread;

  logger.print_state_transition(event, event->thread->previous_state, event->thread->current_state);
}

void Simulation::handle_thread_dispatch_completed(const Event* event) {
  handle_dispatch_completed(event);
}

void Simulation::handle_process_dispatch_completed(const Event* event) {
  handle_dispatch_completed(event);
}

void Simulation::handle_cpu_burst_completed(const Event* event) {
  // Pop burst
  event->thread->bursts.pop();

  // Unset current thread, invoke dispatcher
  last_thread = active_thread;
  active_thread = nullptr;
  events.push(new Event(Event::DISPATCHER_INVOKED, event->time, event->thread));

  if (event->thread->bursts.empty()) {
    // Last burst
    events.push(new Event(Event::THREAD_COMPLETED, event->time, event->thread));
  } else {
    event->thread->set_blocked(event->time + event->thread->bursts.front()->length);
    events.push(new Event(Event::IO_BURST_COMPLETED, event->time + event->thread->bursts.front()->length, event->thread));
    logger.print_state_transition(event, event->thread->previous_state, event->thread->current_state);
  }

}

void Simulation::handle_thread_preempted(const Event* event) {
  // Set ready
  event->thread->set_ready(event->time);
  // Enqueue
  scheduler->enqueue(event, event->thread);
  // Decrease CPU burst
  event->thread->bursts.front()->length = event->thread->bursts.front()->length - time_slice;

  // Invoke dispatcher
  if (!active_thread) {
    events.push(new Event(Event::DISPATCHER_INVOKED, event->time, event->thread));
  }

  logger.print_state_transition(event, event->thread->previous_state, event->thread->current_state);
}

void Simulation::handle_io_burst_completed(const Event* event) {
  // Set ready
  event->thread->set_ready(event->time);
  // Enqueue
  scheduler->enqueue(event, event->thread);
  // Pop burst
  event->thread->bursts.pop();

  // Invoke dispatcher
  if (!active_thread) {
    events.push(new Event(Event::DISPATCHER_INVOKED, event->time, event->thread));
  }

  logger.print_state_transition(event, event->thread->previous_state, event->thread->current_state);
}

void Simulation::handle_thread_completed(const Event* event) {
  event->thread->set_finished(event->time);
  if (active_thread) {
    last_thread = active_thread;
    active_thread = nullptr;
  }
  //events.push(new Event(Event::DISPATCHER_INVOKED, event->time, event->thread));
  logger.print_state_transition(event, event->thread->previous_state, event->thread->current_state);
}
