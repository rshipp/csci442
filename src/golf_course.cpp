#include "golf_course.h"

using namespace std;


GolfCourse::GolfCourse()  {
  for (size_t i = 0; i < 18; i++) {
    // TODO: This is where you will initialize your barriers, condition
    // variables, and any other shared variables or synchronization primitives.
  }
}


double GolfCourse::average_turnaround_time() {
  boost::xtime sum;
  sum.sec = 0;
  sum.nsec = 0;

  vector<boost::xtime>::iterator it;

  for (auto individual_result : turnaround_times) {
    sum.sec += individual_result.sec;
    sum.nsec += individual_result.nsec;
  }

  return (sum.sec + sum.nsec / 1e9) / turnaround_times.size();
}
