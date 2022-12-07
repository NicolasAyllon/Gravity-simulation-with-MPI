#include "mpi.h"

#include <iostream>
#include "argparse.h"
#include "io.h"
#include "quadtree.h"

int main(int argc, char* argv[]) {

  // Get options
  struct options_t opts;
  get_opts(argc, argv, &opts);
  // print_opts(&opts);

  // Read file
  auto particles = read_file(opts.inputfilename);
  // std::cout << "len: " << particles.size() << ", cap: " << particles.capacity() << '\n';
  // for (auto particle : particles) {
  //   std::cout << particle.toStringMatchInputOrder() << '\n';
  // }

  // Create Quadtree for rectangular region (0<=x<=4, 0<=y<=4)
  Region<double> region = {0, 4, 0, 4};
  Quadtree quadtree(region);
  for (auto particle : particles) {
    quadtree.insert(particle);
  }
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