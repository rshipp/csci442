#include <iostream>
#include <queue>
#include "utils/flags.h"
#include "utils/config.h"
#include "abstract/event.h"

using namespace std;

int main(int argc, char** argv) {
  Options options = parse_args(argc, argv);
  Config config = parse_config(options.in_file);

  priority_queue<Event*, vector<const Event*>, EventComparator> events;
  
  vector<Process>::iterator p_it;
  for(p_it = config.processes.begin(); p_it != config.processes.end(); p_it++) {
    vector<Thread*>::iterator t_it;
    for(t_it = p_it->threads.begin(); t_it != p_it->threads.end(); t_it++) {
      cout << "loop 2" << endl;
      events.push(new Event(Event::THREAD_ARRIVED, (*t_it)->arrival_time, *t_it));
    }
  }

  return 0;
}
