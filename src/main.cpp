/**
 * Contains the main() routine of what will eventually be your version of top.
 */

#include <cstdlib>
#include <map>
#include <unistd.h>
#include <ncurses.h>
#include "info/system_info.h"
#include "utils/formatting.h"
#include "utils/flags.h"
#include "utils/sort_functions.h"

using namespace std;


/**
 * Gets a character from the user, waiting for however many milliseconds that
 * were passed to timeout() below. If the letter entered is q, this method will
 * exit the program.
 */
void exit_if_user_presses_q() {
  char c = getch();

  if (c == 'q') {
    endwin();
    exit(EXIT_SUCCESS);
  }
}

void set_cpu_utilization(
    vector<ProcessInfo>& processes,
    const map<int, ProcessInfo>& prev_processes,
    unsigned long long period_in_ticks) {
  // For each process P
  for(vector<ProcessInfo>::iterator it = processes.begin(); it != processes.end(); ++it) {
    // Attempt to find P in prev_processes
    if(prev_processes.count(it->pid) > 0) {
      // If found, set P.cpu_percent to:
      // d(utime + stime) / period_in_ticks * 100%
      it->cpu_percent = ((it->utime + it->stime) - (prev_processes.at(it->pid).utime + prev_processes.at(it->pid).stime)) / period_in_ticks * 100;
    } else {
      // set P.cpu_percent = 0
      it->cpu_percent = 0;
    }
  }
}

map<int, ProcessInfo> make_process_map(vector<ProcessInfo> processes) {
  map<int, ProcessInfo> process_map;
  for(vector<ProcessInfo>::iterator it = processes.begin(); it != processes.end(); ++it) {
    process_map[it->pid] = *it;
  }
  return process_map;
}

/**
 * Entry point for the program.
 */
int main(int argc, char** argv) {
  // parse args
  Options options = parse_args(argc, argv);

  // ncurses initialization
  initscr();

  // Don't show a cursor.
  curs_set(FALSE);

  // Set getch to return after 1000 milliseconds; this allows the program to
  // immediately respond to user input while not blocking indefinitely.
  timeout(options.delay * 100);

  SystemInfo prev_system;
  SystemInfo system = get_system_info();
  while (true) {
    wclear(stdscr);

    // Get system info
    prev_system = system;
    system = get_system_info();
    

    // Calculate CPU utilization
    CpuInfo diff = system.cpus[1] - prev_system.cpus[1];
    unsigned long long delta_ticks = diff.total_time();
    // Calculate CPU utilization for each process
    set_cpu_utilization(system.processes, make_process_map(prev_system.processes), delta_ticks);

    // sort
    sortProcesses(system.processes, options.sort);

    // CURSES
    // uptime
    printw("up %s    ", humanize_uptime(system.uptime).c_str());
    // loadavg
    printw("load average: %.2f, %.2f, %.2f\n", system.load_average.one_min, system.load_average.five_mins, system.load_average.fifteen_mins);
    // processor time
    printw("%-5s %-5s %-5s %-5s\n", "CPU", "USR", "KRN", "IDL");
    for(int i=1; i<system.cpus.size(); i++) {
      CpuInfo delta = system.cpus[i] - prev_system.cpus[i];
      printw("cpu%d  %-5d %-5d %-5d\n", i-1, 100*delta.user_time/delta.total_time(), 100*delta.system_time/delta.total_time(), 100*delta.idle_time/delta.total_time());
    }
    // processes
    printw("Tasks: %d\n", system.processes.size());
    printw("Running: %d\n", system.num_running);
    printw("Threads: %d\n", system.num_threads);
    // memory
    printw("Memory: %s\n", humanize_bytes(system.memory_info.total_memory*1024).c_str());
    printw("Used: %s\n", humanize_bytes((system.memory_info.total_memory - system.memory_info.free_memory)*1024).c_str());
    printw("Free: %s\n", humanize_bytes(system.memory_info.free_memory*1024).c_str());
    // columns
    printw("%-5s %-7s %-7s %-5s %-9s  %s\n", "PID", "RSS", "STATE", "\%CPU", "TIME", "CMD");
    long page_size = sysconf(_SC_PAGESIZE);
    for(int i=0; i<system.processes.size() && i<=20; i++) {
      ProcessInfo p = system.processes[i];
      printw("%-5d %-7s %-7c %-5.1f %-9s  %s\n", p.pid, humanize_bytes(p.rss*page_size).c_str(), p.state, p.cpu_percent, humanize_time(p.utime+p.stime).c_str(), p.comm);
    }


    // Redraw the screen.
    refresh();

    // End the loop and exit if Q is pressed
    exit_if_user_presses_q();
  }

  // ncurses teardown
  endwin();

  return EXIT_SUCCESS;
}
