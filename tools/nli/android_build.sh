#!/bin/bash
export ANDROIDNDK=/media/lichen/4T-data/workspace/3third/android-ndk-r25b
export LD_LIBRARY_PATH=${ANDROIDNDK}/platforms/android-23/arch-arm64/usr/lib:$LD_LIBRARY_PATH

ANDROIDABI="arm64-v8a" #armeabi-v7a 

for ANDROID_ABI in $ANDROIDABI
do
	rm -rf build
	mkdir build && cd build
	cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROIDNDK/build/cmake/android.toolchain.cmake \
		-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=../../../output/Android/${ANDROID_ABI} \
		-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=../../../output/Android/${ANDROID_ABI} \
		-DANDROID_ABI="$ANDROID_ABI" \
		-DPLATFORM="Android" \
		-DANDROID_NATIVE_API_LEVEL=android-21 \
		-DANDROID_ARM_NEON=TRUE \
		-DANDROID_STL=c++_static \
		-DANDROID_TOOLCHAIN=clang \
		-DCMAKE_BUILD_TYPE=RELEASE \
		..
	make -j$(nproc) && make install 
        cd ..
done
rm -rf build

