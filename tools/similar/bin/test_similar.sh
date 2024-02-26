#!/bin/bash

export LD_LIBRARY_PATH=../../3third/so/Linux:$LD_LIBRARY_PATH
# 参数1：模型目录
# 参数2：输入图片目录
# 参数3：输出结果目录
# 参数4：相似度阈值

rm -rf data/results
mkdir -p data/results

./Linux_similar 1 openvino/similar/ data/images data/results 0





