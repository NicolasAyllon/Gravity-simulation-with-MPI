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

  //////////////////////////////////////////////////////////////////////////////
  // Root: Read file into particle vector & broadcast num_particles
  // Non-root: Prepare 0-initialized vectors using the received length
  //////////////////////////////////////////////////////////////////////////////
  // Root process reads number of particles
  int N_particles = 0;
  if (rank == 0) {
    N_particles = read_num_particles(opts.inputfilename);
  }
  // Synchronization point: MPI_Bcast is blocking
  MPI_Bcast(&N_particles, 1, MPI_INT, 0, MPI_COMM_WORLD);
  // Now all processes have the same value of number of particles
  printf("[Process %d] N_particles = %d\n", rank, N_particles);
  // All processes prepare an initialized vector to hold that many particles
  std::vector<Particle> particles(N_particles);
  // Root process reads particles into its particles vector & broadcasts
  if (rank == 0) {
    particles = read_file(opts.inputfilename);
  }

  //////////////////////////////////////////////////////////////////////////////
  // Set up indices in particles array, and args for MPI_Gatherv
  //////////////////////////////////////////////////////////////////////////////
  //
  // Each process will calculate forces for a section of the particles vector.
  // Use rank to calculate the start and end indices of its part.
  // Divide particles among processes, rounding up to cover all particles.
  // This process will calculate forces for particles with index [start, end)
  //
  // Create recvcounts & displacements arrays to use in MPI_Gatherv
  int* recvcounts = (int*)malloc(size * sizeof(int));
  int* displacements = (int*)malloc(size * sizeof(int));
  // To divide particles among processes, store start/end indices in arrays
  int* starts = (int*)malloc(size * sizeof(int));
  int* ends = (int*)malloc(size * sizeof(int));
  // Calculate quotient and remainder for dividing particles among processes
  int q = N_particles / size;
  int r = N_particles % size;
  // Fill recvcounts & displacements arrays, in units of bytes: MPI_BYTE
  int idx = 0;
  for (int i = 0; i < size; ++i) {
    starts[i] = idx;
    ends[i] = idx + q + (i < r ? 1 : 0); // add 1 extra to first r processes
    recvcounts[i] = (ends[i] - starts[i]) * sizeof(Particle);
    displacements[i] = starts[i] * sizeof(Particle);
    idx = ends[i];
  }
  // For convenience, each process can save its own start/end indices
  int start = starts[rank];
  int end = ends[rank];
  int count = end - start;
  printf("[Process %d] will calc forces for [%d, %d)\n", rank, start, end);

  //////////////////////////////////////////////////////////////////////////////
  // Core loop
  //////////////////////////////////////////////////////////////////////////////
  // FOR i = 0..STEPS
  for (int s = 0; s < 2; ++s) { // TODO: replace upper bound with opts.steps

    // Root process broadcasts particle data and others receive
    MPI_Bcast(particles.data(), N_particles*sizeof(Particle), 
              MPI_BYTE, 0, MPI_COMM_WORLD);
    printf("[Process %d] Broadcast complete\n", rank);
    printf("[Process %d] vector<Particles>: len %lu, cap %lu\n", rank, particles.size(), particles.capacity());
    // Now all processes have the same data in the particles vector

    for (size_t i = 0; i < particles.size(); ++i) {
      printf("[Process %d] Particle %lu: %s\n", rank, i, particles[i].toString().c_str());
    }

    // All processes independently construct their own quadtrees


    // All processes calculate forces for their section of particles
    for (int i = start; i < end; ++i) {
      particles[i].position.x += 10;
      particles[i].position.y += 20;
    }

    // Synchronization point: MPI_Gatherv is blocking
    // Gather updated particle vector subsections in root process
    // Root process (and only root) requires MPI_IN_PLACE to use the same buffer
    // for input & output (where input=output & already correct/updated)
    if(rank == 0) { // with MPI_IN_PLACE, sendcount & sendtype are ignored
      MPI_Gatherv(MPI_IN_PLACE, count * sizeof(Particle), MPI_BYTE,
                  particles.data(), recvcounts, displacements, MPI_BYTE, 
                  0, MPI_COMM_WORLD);
      printf("Gathering...\n");
    } else { // Other processes must specify sendbuf, sendcount, & sendtype
      MPI_Gatherv(&particles.data()[start], count * sizeof(Particle), MPI_BYTE, 
                  particles.data(), recvcounts, displacements, MPI_BYTE, 
                  0, MPI_COMM_WORLD);
    }
    // [?] Root process receives & gathers updated particle data
    // See if root process (and only root) shows updates made by others
    for (size_t i = 0; i < particles.size(); ++i) {
      printf("[Process %d] Particle %lu: %s\n", rank, i, particles[i].toString().c_str());
    }

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