#include <queue>
#include "abstract/event.h"
#include "common.h"

//Results run_fcfs(priority_queue<Event*, vector<const Event*>, EventComparator> events);
/**
 * A representation of a scheduling queue that uses first-come, first-served.
 */
class FcfsScheduler : public Scheduler {
public:
  virtual SchedulingDecision* get_next_thread(const Event* event) override;

  virtual void enqueue(const Event* event, Thread* thread) override;
};
