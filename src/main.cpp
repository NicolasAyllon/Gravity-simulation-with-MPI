#include "mpi.h"

#include <iostream>
#include "argparse.h"
#include "io.h"
#include "quadtree.h"
#include "particle.h"
#include "physics.h"
#include "vector.h"

int main(int argc, char* argv[]) {

  // Get options
  struct options_t opts;
  get_opts(argc, argv, &opts); // print_opts(&opts);

  // MPI Initialize and get rank/size
  MPI_Init(&argc, &argv);
  int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size; MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Root process reads number of particles & broadcasts
  int N_particles = 0;
  if (rank == 0) {
    N_particles = read_num_particles(opts.inputfilename);
  }
  MPI_Bcast(&N_particles, 1, MPI_INT, 0, MPI_COMM_WORLD);
  // Now all processes have the same value of number of particles
  printf("[Process %d] N_particles = %d\n", rank, N_particles);
  
  // All processes prepare an initialized vector to hold that many particles
  std::vector<Particle> particles(N_particles);
  // Root process reads particles into its particles vector & broadcasts
  if (rank == 0) {
    particles = read_file(opts.inputfilename);
  }
  void* data = particles.data();
  MPI_Bcast(data, N_particles*sizeof(Particle), MPI_BYTE, 0, MPI_COMM_WORLD);
  printf("[Process %d] vector<Particles>: len %lu, cap %lu\n", rank, particles.size(), particles.capacity());
  // Now all processes have the same data in the particles vector

  for (size_t i = 0; i < particles.size(); ++i) {
    printf("[Process %d] Particle %lu: %s\n", rank, i, particles[i].toString().c_str());
  }

  MPI_Finalize();
  return 0; // <!> end here for now
  // All processes end up with a copy of n_particles set in root process


  // // Get options
  // struct options_t opts;
  // get_opts(argc, argv, &opts); print_opts(&opts);

  // // Read file
  // std::vector<Particle> particles = read_file(opts.inputfilename);
  // size_t n_particles = particles.size();
  // // std::cout << "vector<Particle>: len: " << particles.size() << ", cap: " << particles.capacity() << '\n';

  // // For steps = 0..num_steps-1
  // for (int s = 0; s < opts.steps; ++s) {
  //   // std::cout << "s = " << s << '\n';
  //   // Create Quadtree for rectangular region (0<=x<=4, 0<=y<=4)
  //   Region<double> region = {0, 4, 0, 4};
  //   Quadtree quadtree(region);

  //   // Insert particles
  //   // Particles that move outside the region are "lost". 
  //   // They are not inserted into the quadtree and their mass is set to m = -1
  //   // Subsequent stages check for m = -1 to ignore lost particles.
  //   for (Particle& particle : particles) {
  //     // std::cout << "[Loop] about to insert " << particle.toStringMatchInputOrder(true) << '\n';
  //     quadtree.insert(particle);
  //   }

  //   // For each particle, compute force
  //   std::vector<Vec2<double>> forces(n_particles, {0,0});
  //   // for (Vec2<double> f : forces) {
  //   //   std::cout << f.toString() << '\n';
  //   // }
  //   // Alternatively std::vector<Vec2<double>> forces(); forces.reserve(n_particles);
  //   int start = 0; int end = n_particles;
  //   // std::cout << "start: " << start << ", end: " << end << '\n';
  //   for (int i = start; i < end; ++i) {
  //     // calc_net_force(particles[i], quadtree, opts.theta, forces[i]);
  //     // std::cout << "Calculating force for Particle " << i << '\n';
  //     forces[i] = calc_net_force(particles[i], quadtree, opts.theta);
  //     // std::cout << particles[i].toStringMatchInputOrder(true) 
  //     //           << ", Force: " << forces[i].toString() << '\n';
  //   }

  //   // Calculate new position
  //   // std::cout << "start: " << start << ", end: " << end << '\n';
  //   for (int i = start; i < end; ++i) {
  //     particles[i].update(forces[i], opts.dt);
  //   }
  // }

  // // Write output
  // write_file(particles, opts.outputfilename, false);

  return 0; // <!> end here for now

}