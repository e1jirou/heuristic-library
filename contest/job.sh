#!/bin/bash
# command: nohup sh job.sh &

export OMP_NUM_THREADS=20
g++ -std=gnu++20 -O2 -Wall -Wextra -fopenmp -mtune=native -march=native -fconstexpr-depth=2147483647 -fconstexpr-loop-limit=2147483647 -fconstexpr-ops-limit=2147483647 -I /usr/local/opt/boost/include -o a.out main.cpp
./a.out < in.txt > out.txt
rm -f main
