#pragma once
#include <boost/thread/xtime.hpp>
#include <vector>


class GolfCourse {
// PUBLIC API METHODS
public:

  /**
   * Constructor.
   */
  GolfCourse();

  /**
   * Compute the average turnaround time over all the golfers that have
   * completed the course.
   */
  double average_turnaround_time();

// INSTANCE VARIABLES
public:

  // TODO: This is where you will define your barriers, condition variables, and
  // any other shared variables or synchronization primitives. The variables are
  // currently public to keep it simple.

  /**
   * Vector that gets populated with each golfer's turnaround time as they
   * finish the course.
   */
  std::vector<boost::xtime> turnaround_times;
};
