#include "mpi.h" // [?] Does this work?

#include <iostream>
#include "argparse.h"

int main(int argc, char* argv[]) {

  struct options_t opts;
  get_opts(argc, argv, &opts);
  print_opts(&opts);

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