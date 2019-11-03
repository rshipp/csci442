#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include "flags.h"

using namespace std;

Options parse_args(int argc, char **argv) {
  int c;
  int digit_optind = 0;

  Options options; 
  options.delay = 10;
  string sarg;

  while (1) {
     int this_option_optind = optind ? optind : 1;
     int option_index = 0;
     static struct option long_options[] = {
         {"delay",   required_argument, 0,  'd' },
         {"sort-key",required_argument, 0,  's' },
         {"help",    no_argument,       0,  'h' },
         {0,         0,                 0,  0 }
     };

     c = getopt_long(argc, argv, "d:s:h",
              long_options, &option_index);
     if (c == -1)
         break;

     switch (c) {
     case 'h':
         usage(argv[0]);
         break;

     case 'd':
         int delay;
         if((delay = atoi(optarg))) {
           options.delay = delay;
         } else {
           printf("Delay must be an integer value\n");
           exit(EXIT_FAILURE);
         }
         break;

     case 's':
         sarg = string(optarg);
         if(sarg == "PID" || sarg == "CPU" || sarg == "MEM" || sarg == "TIME") {
           options.sort = sarg;
         } else {
           printf("Valid values are PID, CPU, MEM, TIME\n");
           exit(EXIT_FAILURE);
         }
         break;

     case '?':
         exit(EXIT_FAILURE);
     }
  }

  return options;
}

void usage(char* pname) {
  printf("usage: %s [OPTIONS]\n", pname);
  printf("Options:\n");
  printf("  -d, --delay=DELAY\n");
  printf("    Delay between updates, in tenths of seconds\n");
  printf("  -s, --sort-key=COLUMN\n");
  printf("    Sort by this column; one of: PID, CPU, MEM, or TIME\n");
  printf("  -h, --help\n");
  printf("    Display this help message and exit\n");
  exit(EXIT_SUCCESS);
}
