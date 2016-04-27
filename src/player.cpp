#include "player.h"
#include <iostream>
#include <boost/thread/thread.hpp>
#include <ctime>

using namespace std;
using boost::format;


Player::Player(
    int id,
    int party_id,
    string name,
    double mean,
    double sigma,
    GolfCourse* golf_course,
    boost::xtime start_time)
    : id(id),
      party_id(party_id),
      name(name),
      golf_course(golf_course),
      start_time(start_time) {
  boost::mt19937 random_number_generator;

  // Seed the random number generator. Comment out this line if you want to test
  // your code on a constant play durations
  random_number_generator.seed(std::time(0));

  // Set up the generator to emit numbers from the log normal distribution. Once
  // the generator is set up, you get random numbers from it by calling it like
  // a function--that is, using operator().
  log_norm_generator =  new LogNormalGenerator(
      random_number_generator,
      boost::lognormal_distribution<>(mean, sigma));

  atomic_output(
      format("+++++ Player #%d (%s) added to party %d")
          % id
          % name
          % party_id);
}


void Player::operator()() {
  play_course();
}


// I don't think you will need to modfiy this function.
void Player::play_course() {
  // Playing the course simply means playing each hole.
  for (size_t i = 0; i < 18; i++) {
    play_hole(i);
  }

  boost::xtime turnaround = calculate_turnaround_time();
  golf_course->turnaround_times.push_back(turnaround);
  double turnaround_time = turnaround.sec + turnaround.nsec / 1e9;

  atomic_output(
      format("----- Player #%d (%s) finished. Turnaround time = %f seconds.")
          % id
          % name
          % turnaround_time);
}


void Player::play_hole(size_t hole) {
  // This is the function where you will spend the most effort. I left a few
  // fragments from my solution, but it will need lots of modification.

  if (DEBUG) {
    atomic_output(
        format("##### Player #%d (%s) waiting for hole %d")
            % id
            % name
            % hole);
  }

  // Syntax is a little funny because we have a pointer to a generator.
  // Normally, you just call generator()
  double sleep_duration = (*log_norm_generator)();

  if (DEBUG) {
    atomic_output(
        format("Player #%d (%s) playing hole %d will take %f seconds.")
            % id
            % name
            % hole
            % sleep_duration);
  }

  make_thread_sleep(sleep_duration / simulation_speed);

  if (DEBUG) {
    atomic_output(
        format("$$$$$ Player #%d (%s) notified.")
            % id
            % name);
  }
}


void Player::announce_playing(size_t hole) {
  atomic_output(
      format(">>>>> Party number %d is playing hole %d")
          % party_id
          % hole);
}


void Player::announce_leaving(size_t hole) {
  atomic_output(
      format("<<<<< Party number %d is leaving hole %d")
          % party_id
          % hole);
}


void Player::atomic_output(format fmt) {
  // For the remainder of the scope (this method, in this case), acquire the
  // lock using the I/O mutex so output is atomic. This is needed since
  // iostreams are not guaranteed to be thread-safe.
  boost::mutex::scoped_lock io_lock(io_mutex);

  // If you want to see why we need the lock, try commenting out the scoped_lock
  // line above and changing the endl to "\n" (to avoid flushing the stream
  // immediately).
  cout << fmt << endl;
}


boost::xtime Player::calculate_turnaround_time() {
  boost::xtime end_time, turnaround_time;
  boost::xtime_get(&end_time, boost::TIME_UTC_);

  turnaround_time.sec = end_time.sec - start_time.sec;
  turnaround_time.nsec = end_time.nsec - start_time.nsec;

  return turnaround_time;
}
