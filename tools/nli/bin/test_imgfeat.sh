#!/bin/bash

export LD_LIBRARY_PATH=../../3third/so/Linux:$LD_LIBRARY_PATH
# 提取图像特征，建立特征库
# 参数1：模型目录
# 参数2：输入图片目录
# 参数3：输出结果目录

./Linux_imgfeat models/ data/images data/json





