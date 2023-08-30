#1.nli相关算法介绍
* nli相关算法由图像搜索、场景分类和生成影集三部分组成,实现不同的功能

#1.1图像搜索
* 需要提前建立图像特征库 sh test_imgfeat.sh
* 用文本or图片搜索图片   sh test_search.sh

#1.2生成影集
* 需要提前建立文本特征库 sh test_txtfeat.sh
* 把生成的文本特征库(json文件)拷贝到模型目录
* 生成影集		sh test_platform.sh

