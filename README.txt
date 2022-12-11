Hello!

For my Barnes-Hut implementation with MPI, I decided to go with a broadcast and
gather each iteration.

Rather than construct a complex MPI datatype, I ended up sending the individual
bytes as MPI_BYTE.

The instructions ask to report the total time spent. I estimate this
lab was about 60 hours.

Thank you,

-Nicolas