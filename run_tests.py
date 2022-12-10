import subprocess

PROGRAMS = [
  "nbody"
]

# File names without ".txt" extension, for convenient output file naming
INPUTFILES = [
  # "nb-2-close"    ,
  "nb-10"         ,
  # "nb-100"        ,
  # "nb-100000"     
]

# Maybe change to arrays later
STEPS = 1000
THETA = 0.5
DT_TIMESTEP = 0.005

NUM_TESTS = 1

for program in PROGRAMS:
    for filename in INPUTFILES:
        for i in range(NUM_TESTS):
            subprocess.call([
              "bin/{}".format(program),
              "-i", "input/{}.txt".format(filename),
              "-o", "output/{}-{}.txt".format(filename, STEPS),
              # "-V", # visualization is not used
              "-s", str(STEPS),
              "-t", str(THETA),
              "-d", str(DT_TIMESTEP)
            ])
        print("") # newline after each test