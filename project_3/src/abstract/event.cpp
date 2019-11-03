#include <string>
#include "event.h"

using namespace std;

string event_type_str(Event::EventType num) {
  string type;
  switch(num) {
      case Event::THREAD_ARRIVED:
          type = "THREAD_ARRIVED";
          break;
      case Event::THREAD_DISPATCH_COMPLETED:
          type = "THREAD_DISPATCH_COMPLETED";
          break;
      case Event::PROCESS_DISPATCH_COMPLETED:
          type = "PROCESS_DISPATCH_COMPLETED";
          break;
      case Event::CPU_BURST_COMPLETED:
          type = "CPU_BURST_COMPLETED";
          break;
      case Event::IO_BURST_COMPLETED:
          type = "IO_BURST_COMPLETED";
          break;
      case Event::THREAD_COMPLETED:
          type = "THREAD_COMPLETED";
          break;
      case Event::THREAD_PREEMPTED:
          type = "THREAD_PREEMPTED";
          break;
      case Event::DISPATCHER_INVOKED:
          type = "DISPATCHER_INVOKED";
          break;
  }
  return type;
}
