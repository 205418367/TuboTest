#!/bin/bash

export LD_LIBRARY_PATH=../../3third/so/Linux:$LD_LIBRARY_PATH
rm -rf data/results
rm -rf data/debug
mkdir data/results
mkdir data/debug

#ls -lR | grep "^-"| wc -l
# 参数1：200 每次聚类参与人脸数量
# 参数2：2 循环次数（1是全量聚类、2+增量聚类）
# 参数3：模型路径
# 参数4：数据路径
# 参数5：是否检测人脸（0不检测人脸 1检测人脸）
# 参数6：0 CPU; 1 GPU
# 参数7：使用big face(0不使用 1使用)
# 参数8：输入图片的尺寸默认640，可选480与320
./Linux_cluster 200 2 models/ data/images 1 1 0 640






