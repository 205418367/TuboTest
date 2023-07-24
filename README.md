# Face-cluster with scene-recognition
## Linux平台编译与测试

### 一、编译人脸聚类与场景识别
`$ sh linux_build.sh`

### 二、测试人脸聚类 与场景分类
1. 在data目录下创建cluster和results两个目录
2. linux _test.sh解析
> export LC_ALL="C"
> export LD_LIBRARY_PATH=../3third/so/Linux:$LD_LIBRARY_PATH
> rm -rf data/results/*
>  参数1：200 每次聚类参与人脸数量
>  参数2：2 循环次数（1是全量聚类、2+增量聚类）
>  参数3：模型路径
>  参数4：是否检测人脸（1检测人脸、0不检测人脸）
>  参数5：model type(0大模型 1小模型)
> ./Linux_GetFeature 200 2 ../models/face/ 0 0
> ./Linux_scene

3.执行人脸聚类测试脚本
> sh linux_test.sh
> 测试时把 ./Linux_GetFeature 200 2 ../models/face/ 0 0取消注释
> 把数据拷贝至data/cluster目录，指定模型目录，其他参数可以默认

## ARM平台编译与测试
>  sh linux_test.s
> 测试时把./Linux_scene取消注释
> 支持 输入文件夹;  输入文件;  输入视频buffer;  输入视频路径;四种方式
> 默认输入文件夹data/cluster，结果保存在data/results，具体可以看SceneClaasify.cpp

### 1、build
> 在linux平台交叉编译，需要指定编译器路径 

`$ sh arm-linux_build.sh`

### 2、test
> 需要把模型、so文件、可执行文件与图片拷贝到arm
> 人脸聚类与场景识别参数、测试方法与linux一致

`$ sh arm-linux_test.sh`

## 注意事项
* 具体参数在src/face/GetPREFeature.cpp 与 src/scene/SceneClaasify.cpp设置。
* 人脸聚类、比对、搜索的阈值可以设置，默认设置0.53, 推荐0.50~0.60之间
> 阈值越小区分人脸的难度越大, 多个人可能会被认为是同一个人。
* 所有算法的返回值都需要判断，如果返回值不等于0，则不能进行下一步处理。
* 人脸关键点、特征提取函数的返回值不是0，表示没有关键点和特征值。
* 算法内部已经做了人脸质量分筛选，外面调用时可以不做筛选。
* 算法输入的图象尺寸推荐使用原图尺寸，即不对图象进行压缩。
* 人脸聚类返回结果后，如果一个类别下图片的数量少于2或3张（百度是4张）建议不展示。
