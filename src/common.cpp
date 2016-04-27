#include "common.h"
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>


void make_thread_sleep(double secs) {
  boost::xtime xt;
  boost::xtime_get(&xt, boost::TIME_UTC_);

  int int_part = int(secs);
  double nanos = (secs - int_part) * 1000000000;

  xt.sec += int_part;
  xt.nsec += int(nanos);

  boost::thread::sleep(xt);
}
