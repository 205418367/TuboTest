#!/bin/bash

export LD_LIBRARY_PATH=../../3third/so/Linux:$LD_LIBRARY_PATH
rm -rf data/results/
mkdir data/results/

# 以图搜图、以文本搜图
# 参数1：输入类型 0:文本 1:图片
# 参数2：模型目录
# 参数3：特征库路径
# 参数4：关键词 or 图片路径
# 参数5：与特征库相对应的数据目录
# 参数6：搜索结果保存目录

./Linux_search 0 models/ data/json/imgfeat.json 亲情岁月 data/images data/results





