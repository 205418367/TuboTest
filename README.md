接口文档: <https://note.youdao.com/s/c6k38KWF>&#x20;

# 1.nli相关算法介绍

> nli相关算法由图像搜索、场景分类和生成影集三部分组成,实现不同的功能

## 1.1图像搜索

*   需要提前建立图像特征库 sh test\_imgfeat.sh
*   用文本or图片搜索图片   sh test\_search.sh

## 1.2场景分类

*   基于图像搜索实现自定义场景分类

## 1.3生成影集

*   需要提前建立文本特征库 sh test\_txtfeat.sh
*   把生成的文本特征库(json文件)拷贝到模型目录
*   生成影集 sh test\_platform.sh
