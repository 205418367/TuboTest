#!/bin/bash

export LD_LIBRARY_PATH=../../3third/so/Linux:$LD_LIBRARY_PATH
rm -rf data/results
rm -rf data/debug
mkdir data/results
mkdir data/debug


./Linux_infomap 3000 data/images/4.JPG models/ 






