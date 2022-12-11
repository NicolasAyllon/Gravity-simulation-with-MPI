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
  // All processes prepare an initialized vector to hold that many particles
  std::vector<Particle> particles(N_particles);

  // Root reads particles into its particles vector
  if (rank == 0) { 
    particles = read_file(opts.inputfilename); 
  }
  // Start timer (root process only, before 1st broadcast)
  double t_start = 0; double t_end = 0;
  if (rank == 0) { 
    t_start = MPI_Wtime(); 
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
  // printf("[Process %d] will calc forces for [%d, %d)\n", rank, start, end);

  //////////////////////////////////////////////////////////////////////////////
  // Core loop
  //////////////////////////////////////////////////////////////////////////////
  for (int s = 0; s < opts.steps; ++s) {
    // 1. Root process broadcasts new particle data and others receive
    // [Synchronization point: MPI_Bcast is blocking]
    MPI_Bcast(particles.data(), N_particles*sizeof(Particle), 
              MPI_BYTE, 0, MPI_COMM_WORLD);
    // Now all processes have the same data in the particles vector
    // printf("[Process %d] Step %d: Broadcast complete\n", rank, s);

    // 2. All processes independently construct their own quadtrees
    // Create Quadtree for rectangular region (0<=x<=4, 0<=y<=4)
    Region<double> region = {0, 4, 0, 4};
    Quadtree quadtree(region);
    // Insert particles
    // Particles that move outside the region are "lost". 
    // They are not inserted into the quadtree and their mass is set to m = -1
    // Subsequent stages check for m = -1 to ignore lost particles.
    for (Particle& particle : particles) {
      quadtree.insert(particle);
    }

    // 3. All processes calculate forces for their section of particles
    // For each particle, compute force
    std::vector<Vec2<double>> forces(N_particles, {0,0});
    for (int i = start; i < end; ++i) {
      forces[i] = calc_net_force(particles[i], quadtree, opts.theta);
    }

    // 4. All processes calculate updated particle positions for the assigned
    // slice of the particles vector.
    for (int i = start; i < end; ++i) {
      particles[i].update(forces[i], opts.dt);
    }

    // 5. Gather updated particle vector subsections in root process
    // [Synchronization point: MPI_Gatherv is blocking]
    // Root process (and only root) requires MPI_IN_PLACE to use the same buffer
    // for input & output (where input=output & already correct/updated)
    if (rank == 0) { // with MPI_IN_PLACE, sendcount & sendtype are ignored
      MPI_Gatherv(MPI_IN_PLACE, count * sizeof(Particle), MPI_BYTE,
                  particles.data(), recvcounts, displacements, MPI_BYTE, 
                  0, MPI_COMM_WORLD);
      // printf("[Process %d] Gathering...\n", rank);
    } else { // Other processes must specify sendbuf, sendcount, & sendtype
      MPI_Gatherv(&particles.data()[start], count * sizeof(Particle), MPI_BYTE, 
                  particles.data(), recvcounts, displacements, MPI_BYTE, 
                  0, MPI_COMM_WORLD);
    }
  }
  // All steps complete.
  // Stop timer (core loop, root process only) and print output
  if (rank == 0) { 
    t_end = MPI_Wtime();
    double elapsed_seconds = t_end - t_start;
    printf("%f\n", elapsed_seconds);
  }
  // Write output file (root process only)
  if (rank == 0) { 
    write_file(particles, opts.outputfilename, false);
  }
  MPI_Finalize();
  return 0;
}
