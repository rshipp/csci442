#include "formatting.h"
#include <stdio.h>
#include <sstream>

using namespace std;

string humanize_bytes(long bytes) {
  if(bytes < 1024) {
    return to_string(bytes)+"B";
  } else if(bytes < 1024*1024) {
    return to_string(bytes/1024)+"KiB";
  } else if(bytes < 1024*1024*1024) {
    return to_string(bytes/(1024*1024))+"MiB";
  } else {
    return to_string(bytes/(1024*1024*1024))+"GiB";
  }
}

string humanize_time(int ticks) {
  return string();
}

string humanize_uptime(float seconds) {
  char uptime[100];
  int ss = 0, mm = 0, hh = 0, days = 0;

  if (seconds < 60) {
    ss = seconds;
  } else if(seconds < 60*60) {
    mm = seconds/60;
    ss = (int)seconds % 60;
  } else if(seconds < 60*60*24) {
    hh = seconds/(60*60);
    mm = (int)seconds % (60*60) / 60;
    ss = (int)seconds  % 60;
  } else {
    days = seconds/(60*60*24);
    hh = (int)seconds % (60*60*24) / (60*60);
    mm = (int)seconds % (60*60) / 60;
    ss = (int)seconds % 60;
  }

  if(days > 0) {
    sprintf(uptime, "%d days, %02d:%02d:%02d", days, hh, mm, ss);
  } else {
    sprintf(uptime, "%02d:%02d:%02d", hh, mm, ss);
  }
  return string(uptime);
}

string to_string(long num) {
  stringstream ss;
  ss << num;
  return ss.str();
}
