#include "mpi.h"

#include <iostream>
#include "argparse.h"
#include "io.h"
#include "quadtree.h"
#include "physics.h"

int main(int argc, char* argv[]) {

  // Get options
  struct options_t opts;
  get_opts(argc, argv, &opts); print_opts(&opts);

  // Read file
  std::vector<Particle> particles = read_file(opts.inputfilename);
  size_t N = particles.size();
 //  std::cout << "vector<Particle>: len: " << particles.size() << ", cap: " << particles.capacity() << '\n';

  // For steps = 0..num_steps-1
  for (int s = 0; s < opts.steps; ++s) {
    std::cout << "s = " << s << '\n';
    // Create Quadtree for rectangular region (0<=x<=4, 0<=y<=4)
    Region<double> region = {0, 4, 0, 4};
    Quadtree quadtree(region);

    // Insert particles
    for (Particle& particle : particles) {
      // std::cout << "[Loop] about to insert " << particle.toStringMatchInputOrder(true) << '\n';
      quadtree.insert(particle);
    }

    // For each particle, compute force
    std::vector<Vec2<double>> forces(N, {0,0});
    // Alternatively std::vector<Vec2<double>> forces(); forces.reserve(N);
    int start = 0; int end = N;
    for (int i = start; i < end; ++i) {
      calc_net_force(particles[i], quadtree, opts.theta, forces[i]);
      std::cout << particles[i].toStringMatchInputOrder(true) << ", Force: " << forces[i].toString() << '\n';
    }

    // Calculate new position
    for (int i = start; i < end; ++i) {
      particles[i].update(forces[i], opts.dt);
    }
  }

  // Write output
  write_file(particles, opts.outputfilename);

  return 0; // <!> end here for now




  std::cout << "hello world" << std::endl;
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  // MPI_Send()
  MPI_Finalize();
  // return 0;
}