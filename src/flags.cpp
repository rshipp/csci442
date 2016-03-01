#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include "flags.h"

using namespace std;

Options parse_args(int argc, char **argv) {
  int c;
  int digit_optind = 0;

  Options options; 
  string sarg;

  while (1) {
     int this_option_optind = optind ? optind : 1;
     int option_index = 0;
     static struct option long_options[] = {
         {"per_thread", no_argument,       0, 't' },
         {"verbose",    no_argument,       0, 'v' },
         {"algorithm",  required_argument, 0, 'a' },
         {"help",       no_argument,       0, 'h' },
         {0,            0,                 0,  0 }
     };


     c = getopt_long(argc, argv, "tva:h",
              long_options, &option_index);
     if (c == -1)
         break;

     switch (c) {
     case 'h':
         usage(argv[0]);
         break;

     case 't':
         options.per_thread = true;
         break;

     case 'v':
         options.verbose = true;
         break;

     case 'a':
         sarg = string(optarg);
         if(sarg == "FCFS" || sarg == "RR" || sarg == "PRIORITY" || sarg == "CUSTOM") {
           options.algorithm = sarg;
         } else {
           printf("Valid values are FCFS, RR, PRIORITY, CUSTOM\n");
           exit(EXIT_FAILURE);
         }
         break;

     case '?':
         exit(EXIT_FAILURE);
     }
  }

  if (optind < argc) {
    options.in_file = string(argv[optind]);
  } else {
    // Missing filename
    printf("Missing SIMULATION_FILE\n");
    exit(EXIT_FAILURE);
  }

  return options;
}

void usage(char* pname) {
  printf("usage: %s [OPTIONS] SIMULATION_FILE\n", pname);
  printf("Options:\n");
  printf("  -t, --per_thread\n");
  printf("    Output additional per-thread statistics for arrival time, service time, etc.\n");
  printf("  -v, --verbose\n");
  printf("    Output information about every state-changing event and scheduling decision.\n");
  printf("  -a, --algorithm=ALGORITHM\n");
  printf("    The scheduling algorithm to use. One of FCFS, RR, PRIORITY, or CUSTOM.\n");
  printf("  -h, --help\n");
  printf("    Display this help message and exit\n");
  exit(EXIT_SUCCESS);
}
