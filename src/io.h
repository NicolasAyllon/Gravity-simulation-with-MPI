#ifndef _IO_H
#define _IO_H

#include <vector>
#include "particle.h"

// Reads only the first line of the file that contains the number of particles.
int read_num_particles(char* inputfilename);
// Reads the given file and returns the data in a vector of Particles
std::vector<Particle> read_file(char* inputfilename);

// Writes the result vector of Particles to the specified output file.
// Use sci_notation to indicate whether to use scientific notation.
void write_file(const std::vector<Particle>&, char* outputfilename, bool sci_notation);

#endif