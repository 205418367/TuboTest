#pragma once
#include <iostream>

extern "C" {

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

struct tiorb_nli_cluster_info {
    char* content= nullptr;
};
// english
EX_DLL int NliENCluster(const char* jsondir, int numfeatures, float* imgfeatures, tiorb_nli_cluster_info* cluster_info, int numshow=30, float thresh=0.75f);
// chinese 
EX_DLL int NliCluster(const char* jsondir, int numfeatures, float* imgfeatures, tiorb_nli_cluster_info* cluster_info, int numshow=30, float thresh=0.85f);
EX_DLL int NliDestroy(tiorb_nli_cluster_info* cluster_info);

struct tiorb_nli_search_info { 
    int numshow = 30;
    int* indexs = nullptr;
    float* scores = nullptr;
};
EX_DLL int SearchImg(float* target, int numfeats, float* imgfeats, tiorb_nli_search_info* search_info, int numshow=30);
EX_DLL int SearchDestroy(tiorb_nli_search_info* search_info);

struct tiorb_nli_scene_info { 
    int numshow = 0;
    int* indexs = nullptr;
};
EX_DLL int SceneInfer(float* target, int numFeats, float* imgFeats, tiorb_nli_scene_info* sceneInfo, float thresh);
EX_DLL int SceneDestroy(tiorb_nli_scene_info* scene_info);

}
