import subprocess

# Instructions: run with "python3 run_tests.py"

PROGRAMS = [ "nbody" ]
# File names without ".txt" extension, for convenient output file naming
INPUTFILES = [
  # "nb-2-close"    ,
  "nb-10"         ,
  # "nb-100"        ,
  # "nb-100000"     
]

PROCESSES = [ 
  1, 2, 3, 4, 6, 
  # 8, 10, 12, 14, 16
]
STEPS = 1000
THETAS = [
  # 0, 0.25, 
  0.5, 
  # 0.75, 1
]
DT_TIMESTEP = 0.005
NUM_TESTS = 1

for program in PROGRAMS:
    for filename in INPUTFILES:
        for n_processes in PROCESSES:
            for theta in THETAS:
                print("{}-{}/{}".format(filename, STEPS, n_processes))
                for i in range(NUM_TESTS):
                    subprocess.call([
                      "mpirun",
                        "-np", str(n_processes), 
                      "bin/{}".format(program),
                        "-i", "input/{}.txt".format(filename),
                        "-o", "output/mpi/{}-{}-np{}.txt".format(filename, 
                                                                STEPS, 
                                                                n_processes),
                        # "-V", # visualization is not used
                        "-s", str(STEPS),
                        "-t", str(theta),
                        "-d", str(DT_TIMESTEP)
                    ])
                    # print(' ', end='', flush=True) # Python 3
                # print('', end='\n', flush=True) # Python 3
