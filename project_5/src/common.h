#pragma once
#include <boost/thread/barrier.hpp>


/**
 * If true, outputs additional information for debugging.
 */
static const bool DEBUG = true;


/**
 * Set to 1.0 for normal speed, 10.0 for ten times faster, etc.
 */
static const double simulation_speed = 1.0;


/**
 * Mutex used when doing output, since the iostreams are not guaranteed to be
 * thread-safe.
 */
static boost::mutex io_mutex;


/**
 * Causes the thread to sleep for the given number of seconds.
 */
void make_thread_sleep(double secs);
