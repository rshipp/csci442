/**
 * This file contains implementations for methods in the flag_parser.h file.
 *
 * You'll need to add code here to make the corresponding tests pass.
 */

#include "flag_parser/flag_parser.h"
#include <iostream>
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>

using namespace std;


void print_usage() {
  cout <<
      "Usage: mem-sim [options] filename\n"
      "\n"
      "Options:\n"
      "  -v, --verbose\n"
      "      Output information about every memory access.\n"
      "\n"
      "  -s, --strategy (FIFO | LRU)\n"
      "      The replacement strategy to use. One of FIFO or LRU.\n"
      "\n"
      "  -f, --max-frames <positive integer>\n"
      "      The maximum number of frames a process may be allocated.\n"
      "\n"
      "  -h --help\n"
      "      Display a help message about these flags and exit\n"
      "\n";
}


bool parse_flags(int argc, char** argv, FlagOptions& flags) {
  int c;

  string sarg;
  int iarg;

  while (1) {
     int option_index = 0;
     static struct option long_options[] = {
         {"verbose",    no_argument,       0, 'v' },
         {"strategy",   required_argument, 0, 's' },
         {"max-frames", required_argument, 0, 'f' },
         {"help",       no_argument,       0, 'h' },
         {0,            0,                 0,  0 }
     };


     c = getopt_long(argc, argv, "vs:f:h",
              long_options, &option_index);
     if (c == -1)
         break;

     switch (c) {
     case 'h':
         print_usage();
         break;

     case 'v':
         flags.verbose = true;
         break;

     case 's':
         sarg = string(optarg);
         if(sarg == "FIFO" || sarg == "LRU") {
           if(sarg == "LRU") {
             flags.strategy = ReplacementStrategy::LRU;
           }
         } else {
           printf("Valid values are FIFO, LRU\n");
           return false;
         }
         break;

     case 'f':
         iarg = atoi(optarg);
         if(iarg > 1) {
           flags.max_frames = iarg;
         } else {
           printf("Must be a positive integer\n");
           return false;
         }
         break;

     case '?':
         return false;
     }
  }

  if (optind < argc) {
    flags.filename = string(argv[argc-1]);
  } else {
    // Missing filename
    printf("Missing filename\n");
    return false;
  }

  return true;
}
