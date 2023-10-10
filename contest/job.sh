#!/bin/bash
# command: nohup sh job.sh &

export OMP_NUM_THREADS=16
g++ -std=gnu++20 -O3 -Wall -Wextra -fopenmp main.cpp -o main
./main < in.txt > out.txt
rm -f main
