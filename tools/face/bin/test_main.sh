#!/bin/bash

export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
rm -rf data/results
mkdir data/results

./Linux_thread 1 openvino/face data/images/ data/results/


