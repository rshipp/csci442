#pragma once
#include "golf_course.h"
#include "common.h"
#include <boost/format.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/lognormal_distribution.hpp>
#include <boost/thread/xtime.hpp>
#include <string>

class Player {
// PUBLIC API
public:

  /**
   * Constructor.
   */
  Player(
      int id,
      int party_id,
      std::string name,
      double mean,
      double sigma,
      GolfCourse* golf_course,
      boost::xtime start_time);

  /**
   * Causes the player to start golfing. This operator gets called when the
   * thread is created.
   */
  void operator()();

// HELPER METHODS
private:

  /**
   * Starts the player golfing. The operator() method above invokes this
   * better-named method.
   */
  void play_course();

  /**
   * This is the function where you will spend the most effort. I left a few
   * fragments from my solution, but it will need lots of modification.
   */
  void play_hole(size_t hole);

  /**
   * Exactly one player in each party much call this function for each hole.
   */
  void announce_playing(size_t hole);

  /**
   * Exactly one player in each party much call this function for each hole.
   */
  void announce_leaving(size_t hole);

  /**
   * Outputs the given Boost format string as a single atomic operation.
   */
  void atomic_output(boost::format fmt);

  /**
   * Calculates the turnaround time for the player.
   */
  boost::xtime calculate_turnaround_time();

// INSTANCE VARIABLES
private:

  /**
   * Typedef for the log-normal generator. C++ is SO pretty, isn't it? =)
   */
  typedef
      boost::variate_generator<boost::mt19937, boost::lognormal_distribution<>>
      LogNormalGenerator;

  /**
   * The ID of the player.
   */
  int id;

  /**
   * The ID of the party to which this player belongs.
   */
  int party_id;

  /**
   * The name of the player.
   */
  std::string name;

  /**
   * The golf course on which the player is playing.
   */
  GolfCourse* golf_course;

  /**
   * The time at which the player started golfing.
   */
  boost::xtime start_time;

  /**
   * The log-normal random number generator.
   */
  LogNormalGenerator* log_norm_generator;
};
