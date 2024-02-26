#!/bin/bash

rm -rf build && mkdir build && cd build
ARM_ABI="JDSPACE" # AARCH64 YINGLAIARM YINGLAINPU LINARONPU LINAROCPU OPENWRT JBOLONG
PROJECT_PATH=`pwd`

if [ "$ARM_ABI" = "AARCH64" ];then
    SYSTEM=aarch64
    CCTOOL=aarch64-openwrt-linux-gcc
    CXXTOOL=aarch64-openwrt-linux-g++
    export TOOLCHAIN=/media/lichen/4T-data/workspace/3third/toolchain-aarch64_cortex-a53_gcc-8.2.0_glibc
elif [ "$ARM_ABI" = "JDSPACE" ];then
    SYSTEM=riscv64
    PREFIX=riscv64-unknown-linux-gnu
    export TOOLCHAIN=/media/lichen/4T-data/workspace/3third/spacemit-toolchain-linux-glibc-x86_64-v0.3.0
elif [ "$ARM_ABI" = "YINGLAINPU" ];then
    SYSTEM=aarch64
    CCTOOL=aarch64-linux-gnu-gcc
    CXXTOOL=aarch64-linux-gnu-g++
    export TOOLCHAIN=/media/lichen/737b4994-de25-4859-aaed-da1136ebb3a1/workspace2/3third/gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu
elif [ "$ARM_ABI" = "LINARO" ];then
    SYSTEM=aarch64
    CCTOOL=aarch64-linux-gnu-gcc
    CXXTOOL=aarch64-linux-gnu-g++
    export TOOLCHAIN=/media/lichen/737b4994-de25-4859-aaed-da1136ebb3a1/storage/3third/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu
elif [ "$ARM_ABI" = "OPENWRT" ];then
    SYSTEM=x86_64
    CCTOOL=x86_64-openwrt-linux-gcc
    CXXTOOL=x86_64-openwrt-linux-g++
    export TOOLCHAIN=/media/lichen/737b4994-de25-4859-aaed-da1136ebb3a1/storage/3third/toolchain-x86_64/toolchain-x86_64_gcc-8.4.0_glibc
    export TOOLCHAIN2=/media/lichen/737b4994-de25-4859-aaed-da1136ebb3a1/storage/3third/toolchain-x86_64/target-x86_64_glibc
elif [ "$ARM_ABI" = "OPENWRTNPU" ];then
    SYSTEM=aarch64
    CCTOOL=aarch64-openwrt-linux-gcc
    CXXTOOL=aarch64-openwrt-linux-g++
    export TOOLCHAIN=/media/lichen/737b4994-de25-4859-aaed-da1136ebb3a1/storage/3third/1619b_staging_dir/toolchain-aarch64_cortex-a55_gcc-8.2.0_glibc
    export TOOLCHAIN2=/media/lichen/737b4994-de25-4859-aaed-da1136ebb3a1/storage/3third/1619b_staging_dir/target-aarch64_cortex-a55_glibc
elif [ "$ARM_ABI" = "JBOLONG" ];then
    SYSTEM=arm
    CCTOOL=arm-linux-gnueabihf-gcc
    CXXTOOL=arm-linux-gnueabihf-g++
    export TOOLCHAIN=/media/lichen/737b4994-de25-4859-aaed-da1136ebb3a1/storage/3third/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabihf
elif [ "$ARM_ABI" = "HMEIDI" ];then
    SYSTEM=aarch64
    CCTOOL=aarch64-openwrt-linux-gcc
    CXXTOOL=aarch64-openwrt-linux-g++
    export TOOLCHAIN=/media/lichen/737b4994-de25-4859-aaed-da1136ebb3a1/storage/3third/toolchain-aarch64_cortex-a55_gcc-8.2.0_glibc
elif [ "$ARM_ABI" = "UODIZN" ];then
    SYSTEM=aarch64
    CCTOOL=aarch64-linux-gcc
    CXXTOOL=aarch64-linux-g++
    export TOOLCHAIN=/media/lichen/737b4994-de25-4859-aaed-da1136ebb3a1/storage/3third/uodizh
fi

export STAGING_DIR=$TOOLCHAIN/bin:$STAGING_DIR
if [ "$ARM_ABI" = "OPENWRT" ] || [ "$ARM_ABI" = "OPENWRTNPU" ];then
    export CXXFLAGS="$CXXFLAGS -Os -fPIC -D_GLIBCXX_USE_CXX11_ABI=0 -I${TOOLCHAIN}/include -I${TOOLCHAIN2}/usr/include -I${TOOLCHAIN2}/usr/include/glib-2.0"
    export LDFLAGS="-L${TOOLCHAIN}/lib -L${TOOLCHAIN2}/usr/lib -static-libstdc++ -Wl,-rpath='\$\$ORIGIN/'"
else
    export CXXFLAGS="$CXXFLAGS -Os -fPIC -D_GLIBCXX_USE_CXX14_ABI=1 -I${TOOLCHAIN}/include"
    export LDFLAGS="-L${TOOLCHAIN}/lib -static-libgcc -static-libstdc++ -Wl,-rpath=${TOOLCHAIN}/lib" # -Wl,-rpath=${TOOLCHAIN}/lib
fi

{
    echo "#################### $ARM_ABI ####################"
    cmake .. \
    -DPLATFORM=ARM \
    -DARM_ABI=${ARM_ABI} \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_SYSTEM_VERSION=1 \
    -DCMAKE_SYSTEM_PROCESSOR=$SYSTEM \
    -DCMAKE_C_COMPILER=$TOOLCHAIN/bin/$PREFIX-gcc \
    -DCMAKE_CXX_COMPILER=$TOOLCHAIN/bin/$PREFIX-g++
}

make -j2 && make install  
cp Linux_platform ../bin/
cp Linux_embedding ../bin/
cp Linux_search ../bin/
cd .. && rm -rf build


