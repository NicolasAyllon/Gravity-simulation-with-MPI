import subprocess

PROGRAMS = [
  "nbody"
]

IOFILES = {
  "nb-10.txt"     : "nb-10-output.txt"      #,
  #  "nb-100.txt"    : "nb-100-output.txt"    #,
  # "nb-100000.txt" : "nb-100000-output.txt"
}

# Maybe change to arrays later
STEPS = 1000
THETA = 0.5
DT_TIMESTEP = 0.005

NUM_TESTS = 1

for program in PROGRAMS:
    for inputfilename, outputfilename in IOFILES.items():
        for i in range(NUM_TESTS):
            subprocess.call([
              "bin/{}".format(program),
              "-i", "input/{}".format(inputfilename),
              "-o", "output/{}".format(outputfilename),
              # "-V", # visualization is not used
              "-s", str(STEPS),
              "-t", str(THETA),
              "-d", str(DT_TIMESTEP)
            ])
        print("") # newline after each test