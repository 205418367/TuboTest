#!/bin/bash

export LD_LIBRARY_PATH=../../../3third/so/Linux:$LD_LIBRARY_PATH

rm -rf data/results
mkdir data/results

./Linux_merge data/json3/config.json data/results/





