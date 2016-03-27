# Project 3

- Your name.
  * Ryan Shipp
- A list of all the files in your submission and what each does.
```
makefile
|- Build instructions
README.md
|- This file
src/abstract/burst.h
|- Burst abstraction
src/abstract/event.cpp
|- Event abstraction
src/abstract/event.h
|- Event header
src/abstract/process.cpp
|- Process abstraction
src/abstract/process.h
|- Process header
src/abstract/scheduling_decision.h
|- Scheduling decision abstraction
src/abstract/thread.cpp
|- Thread abstraction
src/abstract/thread.h
|- Thread header
src/alg/custom.cpp
|- Custom algorithm
src/alg/custom.h
|- Custom alg header
src/alg/fcfs.cpp
|- FCFS algorithm
src/alg/fcfs.h
|- FCFS header
src/alg/priority.cpp
|- Priority algorithm
src/alg/priority.h
|- Priority header
src/alg/rr.cpp
|- Round robin algorithm
src/alg/rr.h
|- Round robin header
src/alg/scheduler.h
|- Scheduler base class
src/alg/simulation.cpp
|- Simulation abstraction and logic
src/alg/simulation.h
|- Simulation header
src/main.cpp
|- Main function
src/utils/config.cpp
|- Config parsing logic
src/utils/config.h
|- Config header
src/utils/flags.cpp
|- Argument parsing logic
src/utils/flags.h
|- Argument parser header
src/utils/logger.cpp
|- Logging logic
src/utils/logger.h
|- Logging header
src/utils/system_stats.cpp
|- System stats logic
src/utils/system_stats.h
|- System stats header
```
- Any unusual/interesting features in your programs.
  * No
- Approximate number of hours you spent on the project.
  * ~40?
- A short essay that explains your custom CPU scheduling algorithm. Make sure to cover:
  * How are processes of different priorities handled?
  * What metrics did you try to optimize (e.g. throughput, response time, etc)?
  * How does your algorithm use preemption?
  * How do you make use of the required number of queues?
  * Assuming a constant stream of processes, is starvation possible in your algorithm?
  * Is your algorithm fair? What does that even mean?

## Custom Algorithm

The custom algorithm is an implementation of a multilevel feedback queue
scheduler (MLFQ). It contains six queues, and uses preemption with a time
slice of 8. Each time a thread is preempted, it is moved to a lower priority
queue, until it reaches the bottom and is moved back to the first queue
(assuming it remains through that many preemptions). Processes are initially
added to a queue based on their priority, giving higher priority processes
first access to the CPU. The algorithm attempts to optimize throughput for
I/O bound processes, and gives preference to shorter jobs. Long processes
sink down the six queues as they continue to be preempted and demoted.
Assuming a constant stream of processes, starvation is not possible in this
algorithm - it functions similarly to round robin, as processes eventually
cycle up from the sixth queue to the first. The algorithm tries to be fair,
meaning it will eventually give equal CPU time to each process. Shorter
processes may finish quickly before being preempted, but longer
processes are also given time slices until they finish, with even higher
priority threads being demoted just as lower priority threads are.
