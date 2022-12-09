#include "mpi.h"

#include <iostream>
#include "argparse.h"
#include "io.h"
#include "quadtree.h"
#include "particle.h"
#include "physics.h"
#include "vector.h"

int main(int argc, char* argv[]) {

  std::cout << "hello world" << std::endl;
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::cout << "I am process " << rank << " of " << size << '\n';

  // MPI send/receive of std::vector<Particle> needs Particle and Vec2 defined.
  // Define Vec2
  // MPI_Datatype dt_Vec2;
  // MPI_Type_contiguous(2, MPI_DOUBLE, &dt_Vec2);
  // MPI_Type_commit(&dt_Vec2);
  // Define Particle
  // (not yet implemented)

  if (rank == 0) {
    std::vector<Particle> v_test;
    v_test.reserve(3);
    v_test.emplace_back(Particle{0, 3.1, {1.2, 2.2}, {0, 0}});
    v_test.emplace_back(Particle{0, 4.2, {8.2, 4.2}, {0, 0}});
    v_test.emplace_back(Particle{0, 5.9, {3.2, 7.2}, {0, 0}});

    int N = v_test.size();
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    void* data = v_test.data(); 
    int n_bytes = N * sizeof(Particle);
    MPI_Bcast(data, n_bytes, MPI_BYTE, 0, MPI_COMM_WORLD);
  }
  else {
    int N = 0;
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received N = %d\n", rank, N);

    std::vector<Particle> v(N);
    void* data = v.data();
    int n_bytes = N * sizeof(Particle);
    MPI_Bcast(data, n_bytes, MPI_BYTE, 0, MPI_COMM_WORLD);
    printf("Process %d received vector<Particle>: size %lu, cap %lu\n", rank, v.size(), v.capacity());
    for (int i = 0; i < N; ++i) {
      printf("Process %d: v[%d] = %s\n", rank, i, v[i].toStringMatchInputOrder(true).c_str());
    }
  }

  // Rank 0 (Coordinator)
    // 1. Send size to all

    // 2. Send data to all

  // Rank > 0 (Worker)
    // 1. Receive size

    // 2. Receive data

  MPI_Finalize();
  return 0; // <!> end for test MPI

  // Get options
  struct options_t opts;
  get_opts(argc, argv, &opts); print_opts(&opts);

  // Read file
  std::vector<Particle> particles = read_file(opts.inputfilename);
  size_t N = particles.size();
  // std::cout << "vector<Particle>: len: " << particles.size() << ", cap: " << particles.capacity() << '\n';

  // For steps = 0..num_steps-1
  for (int s = 0; s < opts.steps; ++s) {
    // std::cout << "s = " << s << '\n';
    // Create Quadtree for rectangular region (0<=x<=4, 0<=y<=4)
    Region<double> region = {0, 4, 0, 4};
    Quadtree quadtree(region);

    // Insert particles
    // Particles that move outside the region are "lost". 
    // They are not inserted into the quadtree and their mass is set to m = -1
    // Subsequent stages check for m = -1 to ignore lost particles.
    for (Particle& particle : particles) {
      // std::cout << "[Loop] about to insert " << particle.toStringMatchInputOrder(true) << '\n';
      quadtree.insert(particle);
    }

    // For each particle, compute force
    std::vector<Vec2<double>> forces(N, {0,0});
    // for (Vec2<double> f : forces) {
    //   std::cout << f.toString() << '\n';
    // }
    // Alternatively std::vector<Vec2<double>> forces(); forces.reserve(N);
    int start = 0; int end = N;
    // std::cout << "start: " << start << ", end: " << end << '\n';
    for (int i = start; i < end; ++i) {
      // calc_net_force(particles[i], quadtree, opts.theta, forces[i]);
      // std::cout << "Calculating force for Particle " << i << '\n';
      forces[i] = calc_net_force(particles[i], quadtree, opts.theta);
      // std::cout << particles[i].toStringMatchInputOrder(true) 
      //           << ", Force: " << forces[i].toString() << '\n';
    }

    // Calculate new position
    // std::cout << "start: " << start << ", end: " << end << '\n';
    for (int i = start; i < end; ++i) {
      particles[i].update(forces[i], opts.dt);
    }
  }

  // Write output
  write_file(particles, opts.outputfilename, false);

  return 0; // <!> end here for now

}