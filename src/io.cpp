#include "io.h"
#include "particle.h"
#include "vector.h"
#include <iostream>
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
  // std::cout << "num_particles: " << num_particles << '\n';

  // Create a vector with this capacity reserved
  std::vector<Particle> particles;
  particles.reserve(num_particles);

  // Reuse line to read data for all particles
  while(std::getline(ifs, line)) {
    // std::cout << "line: " << line << '\n';
    // Ignore empty lines
    if (line.empty()) { continue; }
    std::stringstream ss(line);
    // Each line contains ordered data:
    Particle p; // uninitialized
    // std::cout << "before initialization: " << p.toString() << '\n';
    ss >> p.index;
    ss >> p.position.x >> p.position.y;
    ss >> p.mass;
    ss >> p.velocity.x >> p.velocity.y;
    // std::cout << "after initialization: " << p.toString() << '\n';
    // Add to vector
    particles.emplace_back(p);
  }
  return particles;
}

void write_file(const std::vector<Particle>& particles, char* outputfilename) {
  // Overwrite the file's previous content
  std::ofstream ofs(outputfilename, std::ios_base::trunc);
  if (!ofs.is_open()) {
    perror("Unable to open file");
    exit(EXIT_FAILURE);
  }
  // TODO: Print time
  // Print number of particles on first line
  ofs << particles.size() << '\n';
  // Print particle index, position, mass, and velocity on each line
  for (const Particle& particle : particles) {
    ofs << particle.index << " "
        << particle.position.x << " " << particle.position.y << " "
        << particle.mass << " "
        << particle.velocity.x << " " << particle.velocity.y << '\n';
  }
}