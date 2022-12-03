#ifndef _ARGPARSE_H
#define _ARGPARSE_H

#include <getopt.h>
#include <iostream>
#include <sstream>
#include <vector>

struct options_t {
  char* inputfilename;    // -i: input filename
  char* outputfilename;   // -o: output filename
  int steps;              // -s: number of iterations
  double theta;           // -t: threshold ratio S/D
                          //     S = region side length
                          //     D = distance to center of mass
  double dt;              // -d: timestep (seconds)
  bool visualization;     // -V: (OPTIONAL) flag for visualization window
                          //     false -> no visualization (default)
                          //     true  -> use visualization
};

void print_opts(struct options_t* opts);

void set_default_opts(struct options_t* opts);

bool contains_undefined_opts(struct options_t* opts);

void get_opts(int argc, char** argv, struct options_t* opts);

#endif