export LC_ALL="C"
export LD_LIBRARY_PATH=../../3third/so:$LD_LIBRARY_PATH
rm -rf data/results
rm -rf data/debug
mkdir data/results
mkdir data/debug

./Linux_thread 4 ../../models data/images data/results/ data/results 0.53
