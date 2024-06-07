#!/bin/bash

export LD_LIBRARY_PATH=../../3third/so:$LD_LIBRARY_PATH
rm -rf data/results
rm -rf data/debug
mkdir data/results
mkdir data/debug

#ls -lR | grep "^-"| wc -l
# 参数1：200 每次聚类参与人脸数量
# 参数2：2 循环次数（1是全量聚类、2+增量聚类）
# 参数3：模型路径
# 参数4：数据路径
# 参数5：封面路径
# 参数6：聚类阈值，默认0.53
./Linux_cluster 200 2 4 models/ data/cluster data/results 0.53






