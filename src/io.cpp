#include "io.h"
#include "particle.h"
#include "vector.h"
#include <fstream>
#include <sstream>
#include <vector>

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
    std::stringstream ss(line);
    // Each line contains ordered data:
    Particle p;
    ss >> p.index;
    ss >> p.position.x >> p.position.y;
    ss >> p.mass;
    ss >> p.velocity.x >> p.velocity.y;
    // Add to vector
    particles.emplace_back(p);
  }
  return particles;
}