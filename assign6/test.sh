#!/bin/bash
NP="${NP:-1}"
make
mpirun -np $NP ./k_means -f ./data/example1_k2_m10.txt -k 2 -i 100
# mpirun -np $NP ./k_means -f ./data/example3_k5_m500.txt -k 5 -i 100