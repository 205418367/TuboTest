export LC_ALL="C"
export LD_LIBRARY_PATH=../../../so/Linux:$LD_LIBRARY_PATH
rm -rf data/results/*
./Linux_cartoon 1.jpg images/results ../../../models/cartoon/
