#!/bin/bash

export LD_LIBRARY_PATH=../../3third/so/Linux:$LD_LIBRARY_PATH
# 参数1：模型目录
# 参数2：输入图片目录
# 参数3：输出结果目录

TARGET=/media/lichen/4T-data/workspace/WisdmPhoto/TiPhoto/TuboTest/tools/blur/bin/data/images
OUTPUT=/media/lichen/4T-data/workspace/WisdmPhoto/TiPhoto/TuboTest/tools/blur/bin/data/results

rm -rf ${OUTPUT}
mkdir -p ${OUTPUT}/sharp
mkdir -p ${OUTPUT}/blur
mkdir -p ${OUTPUT}/norm

./Linux_blur 4 models/ ${TARGET} ${OUTPUT}





