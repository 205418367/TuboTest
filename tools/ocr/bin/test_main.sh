#!/bin/bash

export LD_LIBRARY_PATH=../../../3third/so/Linux:$LD_LIBRARY_PATH

rm -rf data/results
mkdir data/results

./main models/ data/images/证件/data data/results/





