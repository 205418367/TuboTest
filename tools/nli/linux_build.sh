#/bin/bash

PLATFORM="Linux"

rm -rf build
mkdir build && cd build
cmake -DPLATFORM=$PLATFORM ..
make -j2 && make install  
 
#cp Linux_platform ../bin/
#cp Linux_imgfeat ../bin/
#cp Linux_txtfeat ../bin/
#cp Linux_search ../bin/
cd .. && rm -rf build
