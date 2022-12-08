#include "mpi.h"

#include <iostream>
#include "argparse.h"
#include "io.h"
#include "quadtree.h"

int main(int argc, char* argv[]) {

  // Get options
  struct options_t opts;
  get_opts(argc, argv, &opts); // print_opts(&opts);

  // Read file
  std::vector<Particle> particles = read_file(opts.inputfilename);
  // size_t N = particles.size();
 //  std::cout << "vector<Particle>: len: " << particles.size() << ", cap: " << particles.capacity() << '\n';

  // Create Quadtree for rectangular region (0<=x<=4, 0<=y<=4)
  Region<double> region = {0, 4, 0, 4};
  Quadtree quadtree(region);
  // Insert particles
  for (Particle& particle : particles) {
    std::cout << "[Loop] about to insert " << particle.toStringMatchInputOrder(true) << '\n';
    quadtree.insert(particle);
  }
  return 0; // <!> end here for now

  // For each particle, compute force
  // std::vector<Vec2<double>> forces;
  // int start = 0; int end = N;
  // for (int i = start; i < end; ++i) {
  //   forces[i] = calculate_force(particles[i], quadtree);
  // }
  // // Calculate new position
  // for (int i = start; i < end; ++i) {
  //   particles[i].update(forces[i], dt);
  // }

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