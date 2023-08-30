#/bin/bash

PLATFORM="Linux"

rm -rf build
mkdir build && cd build
cmake -DPLATFORM=$PLATFORM ..
make -j2 && make install  
 
cd .. && rm -rf build
