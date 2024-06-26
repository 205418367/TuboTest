#!/bin/bash

export LD_LIBRARY_PATH=../../3third/so/Linux:$LD_LIBRARY_PATH
find data/results/ -mindepth 1 -maxdepth 1 -not -name 'imgfeat.json' -exec rm -rf {} +

# 自动生成影集
# 参数1：模型目录
# 参数2：标签列表
# 参数3：输入图片目录
# 参数4：输出图片目录
# 参数5：每类最多展示
# 参数6：聚类阈值

./Linux_platform 4 models/ data/images data/results
