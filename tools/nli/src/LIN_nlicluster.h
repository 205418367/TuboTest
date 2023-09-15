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

static std::vector<std::string> albumfeat = {
        "美好时光","自然风景","城市之光","动物世界","旅行足迹","人物肖像","四季之美","浪漫时刻","家庭欢乐","绚丽色彩",
	"美食之旅","艺术创作","体育精神","喜怒哀乐","时尚风尚","微笑瞬间","夜晚奇观","心灵静谧","自由奔放","幸福时光",
	"快乐回忆","充满活力","美丽花园","动人瞬间","和平宁静","艺术天地","奇妙冒险","激情飞扬","温馨时刻","静谧自然",
	"亲密时光","动感画面","温暖笑容","迷人风景","奇幻世界","创意构图","快乐童年","自然奇观","冒险探索","感悟生活",
	"阳光明媚","自由飞翔","浓情蜜意","青春无悔","美丽瞬间","梦幻之旅","温柔时光","光影流转","快乐时光","城市掠影",
	"动物乐园","心灵之旅","欢乐时刻","阳光海滩","狂野之旅","甜蜜回忆","流光溢彩","夏日时光","云端漫步","绿意盎然",
	"美妙旅程","时光荏苒","奇幻的世界","自然之美","城市风景","欢乐的时刻","美好时光","清新自然","探索无限","自然奇观",
	"动感画面","幸福时光","自由奔放","快乐旅程","大自然的魅力","生命的美好","城市之美","美食佳肴","艺术创作","四季变换",
	"静谧自然","心灵之旅","激情奔放","美丽心情","阳光明媚","欢笑时刻","幸福的瞬间","奇幻之旅","快乐回忆","人与自然",
	"美好时光","自由自在","甜蜜时刻","浪漫之旅","心灵的触动","奇幻探险","美好未来","热情洋溢","夜色迷人","自然奇观",
	"阳光海滩","静谧夜景","沙漠之美","湖光山色","日出日落","雪域仙境","森林探秘","峰巅瀑布","天空之境","峰峦叠嶂",
	"溪流之美","花园物语","郊野风光","湖畔佳境","河畔景色","海风徐来","宝贝成长","欢乐时刻","幸福点滴","家中乐事",
	"趣味记忆","行走天涯","探索未知","旅途回忆","世界探索","自然奇遇","古城漫游","文化探访","乡村漫步","海岛之旅",
	"自然奇观","动物王国","生态探索","季节变迁","创意之旅","创意天地","竞技时刻","运动风采","舞台之光","表演艺术",
	"精彩舞台"
};

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
