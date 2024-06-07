#!/bin/bash

export LD_LIBRARY_PATH=../../3third/so/Linux:$LD_LIBRARY_PATH
find data/results/ -mindepth 1 -maxdepth 1 -not -name 'imgfeat.json' -exec rm -rf {} +

# 以图搜图、以文本搜图
# 参数1：线程数量
# 参数2：模型目录
# 参数3：关键词 or 图片路径
# 参数4：阈值
# 参数5：输入图片的目录
# 参数5：搜索结果保存目录

./Linux_search 4 models/ scene.txt 0.5 data/images data/results





