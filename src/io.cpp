#include "io.h"
#include "particle.h"
#include "vector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Reads only the first line of the file that contains the number of particles.
int read_num_particles(char* inputfilename) {
  std::ifstream ifs(inputfilename);
  if (!ifs.is_open()) {
    perror("Unable to open file");
    exit(EXIT_FAILURE);
  }
  // Get first line that contains the number of points
  int num_particles = 0;
  ifs >> num_particles;

  // Reset stream marker to beginning
  ifs.seekg(0, std::ios_base::beg);
  ifs.close();
  return num_particles;
}

// Reads the input file and returns vector of particles
std::vector<Particle> read_file(char* inputfilename) {
  std::ifstream ifs(inputfilename);
  if (!ifs.is_open()) {
    perror("Unable to open file");
    exit(EXIT_FAILURE);
  }

  // Get first line that contains the number of points
  std::string line;
  std::getline(ifs, line);
  std::stringstream ss(line);
  int num_particles = 0;
  ss >> num_particles;

  // Create a vector with this capacity reserved
  std::vector<Particle> particles;
  particles.reserve(num_particles);

  // Reuse line to read data for all particles
  while(std::getline(ifs, line)) {
    // Ignore empty lines
    if (line.empty()) { continue; }
    std::stringstream ss(line);
    // Each line contains ordered data:
    Particle p; // uninitialized
    ss >> p.index;
    ss >> p.position.x >> p.position.y;
    ss >> p.mass;
    ss >> p.velocity.x >> p.velocity.y;
    // Add to vector
    particles.emplace_back(p);
  }
  return particles;
}

// Writes the final state of all particles to the output file.
// The number of particles is printed on the first line,
// Particle data is printed one per line, and matches the 
// order in which it appeared in the input file.
void write_file(const std::vector<Particle>& particles, char* outputfilename, bool sci_notation) {
  // Overwrite the file's previous content
  std::ofstream ofs(outputfilename, std::ios_base::trunc);
  if (!ofs.is_open()) {
    perror("Unable to open file");
    exit(EXIT_FAILURE);
  }
  // Print number of particles on first line
  ofs << particles.size() << '\n';
  // Switch to scientific notation
  if(sci_notation) {
    ofs.setf(std::ios_base::scientific);
  }
  // Print particle index, position, mass, and velocity on each line
  for (const Particle& particle : particles) {
    ofs << particle.index << " "
        << particle.position.x << " " << particle.position.y << " "
        << particle.mass << " "
        << particle.velocity.x << " " << particle.velocity.y << '\n';
  }
}