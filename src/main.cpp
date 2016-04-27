/**
 * This file contains the program's main() method. It already takes care of
 * reading the input file and spawning the threads, so you shouldn't have to
 * modify this file.
 */

#include "common.h"
#include "player.h"
#include "golf_course.h"
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

using namespace std;


/**
 * Represents a single player's entry in the control file.
 */
struct PlayerFileEntry {
  string name;
  double mean;
  double sigma;
  int sleep;
};


/**
 * Reads the specified file and returns the resulting players.
 */
vector<PlayerFileEntry> read_control_file(char* filename);


/**
 * Entry point to the program.
 */
int main(int argc, char** argv) {

  // Ensure the program was invoked correctly.
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " control-file" << endl;
    return EXIT_FAILURE;
  }

  boost::thread_group threads;
  boost::xtime created_at;

  GolfCourse* golf_course = new GolfCourse();
  vector<PlayerFileEntry> players = read_control_file(argv[1]);

  // Create all the players in the control file, giving them unique ids and
  // assiging them to parties in the order they arrive.
  for (size_t id = 0; id < players.size(); id++) {
    // Use integer division to determine the group ID.
    size_t party_id = id / 4; 

    boost::xtime_get(&created_at, boost::TIME_UTC_);

    // Instantiate a new player. The player will stay in scope in the thread we
    // create below, so there's no need to save it in a data structure.
    Player p(
        id,
        party_id,
        players[id].name,
        players[id].mean,
        players[id].sigma,
        golf_course,
        created_at);

    // Create a new thread that invokes the new player's operator() method.
    // This starts him playing the course.
    threads.create_thread(p);

    // Wait to create the next thread.
    make_thread_sleep(players[id].sleep);
  }

  // Don't let main() exit until all the threads have reached this point.
  threads.join_all();

  // No mutex needed around this output, since we're back to a single thread.
  cout << "Average turnaround time: "
      << golf_course->average_turnaround_time()
      << " seconds."
      << endl;

  return EXIT_SUCCESS;
}


vector<PlayerFileEntry> read_control_file(char* filename) {
  vector<PlayerFileEntry> players;
  ifstream input_file(filename);
  string line;

  if (!input_file) {
    cerr << "Unable to open file: " << filename << endl;
    exit(EXIT_FAILURE);
  }

  while (getline(input_file, line)) {
    PlayerFileEntry entry;

    // Ignore empty lines and comments (lines starting with a '#').
    if (line.size() > 0 && line[0] != '#') {
      istringstream iss(line);
      iss >> entry.name >> entry.mean >> entry.sigma >> entry.sleep;
      players.push_back(entry);
    }
  }

  return players;
}
