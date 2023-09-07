#!/bin/bash

export LD_LIBRARY_PATH=../../3third/so/Linux:$LD_LIBRARY_PATH
# 参数1：模型目录
# 参数2：输入图片目录
# 参数3：输出结果目录

rm -rf data/results
mkdir -p data/results/sharp
mkdir -p data/results/blur

./Linux_blur models/ data/cluster data/results





