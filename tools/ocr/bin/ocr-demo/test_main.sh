#!/bin/bash

export LD_LIBRARY_PATH=./so:$LD_LIBRARY_PATH

rm -rf data/results
mkdir data/results

./main models/ data/images data/results/





