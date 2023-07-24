#pragma once
#ifndef Tiorb_CLIP_IMAGE_CLUSTER_H_
#define Tiorb_CLIP_IMAGE_CLUSTER_H_
#include <iostream>

extern "C" {

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

// 美食、聚餐、烧烤、喝酒、酒吧、餐厅、蛋糕、咖啡、茶、美女、化妆、精致、漂亮、旅游、徒步、露营、登山、野外、篝火    //0 ~18
// 家庭、合影、闺蜜、亲子、同学、儿童、婴儿、爷爷、奶奶、夜景、沙漠、雪景、山峰、湖泊、森林、江河、瀑布、海边、沙滩" //19~37
// cosplay、二次元、手办、动漫、运动、健身、骑行、骑马、跑步、田径场、舞蹈、唱歌、演唱会、相声、音乐、公园、游乐园"  //38~54
// 娱乐设施、校园、学校、寺庙、古镇、园林、桥梁、高速公路、街道、教堂、花朵、绿植、植物、水果、结婚、婚礼、毕业"     //55~71
// 宴会、春节、烟花、庆典、搞怪、跑车、手表、包包、烟酒、赛车、脏乱差、垃圾、杂乱、鬼怪、纯色、空白、工作、屏幕"     //72~89
// 截屏、二维码、文档、文字、办公桌、办公室、课桌、电脑、文本、开会、地铁、公交、出租车、工地、车间、生产、仓库"     //90~106

struct tiorb_nli_cluster_info {
    int num_clusters = 0; 
    int* labels = nullptr;
    int* indexs = nullptr;
    float* scores=nullptr;
};
EX_DLL int NliCluster(const char* jsondir, int numfeatures, float* imgfeatures, tiorb_nli_cluster_info* cluster_info, int numshow=30, float thresh=0.85f);
EX_DLL int NliDestroy(tiorb_nli_cluster_info* cluster_info);

struct tiorb_nli_search_info { 
    int numshow = 30;
    int* indexs = nullptr;
    float* scores = nullptr;
};
EX_DLL int SearchImg(float* target, int numfeats, float* imgfeats, tiorb_nli_search_info* search_info, int numshow=30);
EX_DLL int SearchDestroy(tiorb_nli_search_info* search_info);

}
#endif
