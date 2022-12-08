#ifndef _IO_H
#define _IO_H

#include <vector>
#include "particle.h"

std::vector<Particle> read_file(char* inputfilename);
void write_file(const std::vector<Particle>&, char* outputfilename);

#endif