#/bin/bash

PLATFORM="Linux"

rm -rf build
mkdir build && cd build
cmake .. -DPLATFORM=$PLATFORM 
make -j2 && make install  

cp main ../bin/
cd .. && rm -rf build
