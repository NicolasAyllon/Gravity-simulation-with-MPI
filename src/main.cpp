#include "mpi.h" // [?] Does this work?
#include <iostream>

int main(int argc, char* argv[]) {

  MPI_Init(&argc, &argv);
  std::cout << "hello world" << std::endl;
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  // MPI_Send()
  MPI_Finalize();
  // return 0;
}