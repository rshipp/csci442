#include "load_average_info.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;


LoadAverageInfo get_load_average() {
  // Open the specified file. See @106 for easy ways to create the filenames.
  ifstream loadavg_file(PROC_ROOT "/loadavg");

  // You should check for error. If you can't read the file, it's most likely
  // either a programming error, or your computer doesn't have a proc filesystem.
  // Just print an error and exit if you can't open the file.
  if (!loadavg_file) {
    cerr << "Unable to read from /proc/loadavg" << endl;
    exit(EXIT_FAILURE);
  }

  // Declare a variable into which you can read values.
  LoadAverageInfo load_average;

  // Read the values. The conversion to the appropriate types (doubles, in this
  // case) happens automatically. Yay!
  loadavg_file
      >> load_average.one_min
      >> load_average.five_mins
      >> load_average.fifteen_mins;

  return load_average;
}
