#!/bin/bash

export LD_LIBRARY_PATH=../../3third/so/Linux:$LD_LIBRARY_PATH
rm -rf data/results/
mkdir -p data/results/

# 以图搜图、以文本搜图
# 参数1：线程数量
# 参数2：模型目录
# 参数3：关键词 or 图片路径
# 参数4：阈值
# 参数5：输入图片的目录
# 参数5：搜索结果保存目录

# 推荐的英文关键词与阈值
# pretty cute animal 0.195
# beautiful cute selfie 0.195
# handsome selfie    0.195
# cute child    0.2
# delicious flavorful food 0.19
# beautiful charming scenery 0.195
# lush plants    0.195
# eating together with multiple people    0.2
# intense physical training    0.195

./Linux_search 4 openvino/nli/ "美食" 0 data/images data/results





