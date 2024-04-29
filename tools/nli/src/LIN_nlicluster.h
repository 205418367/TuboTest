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

EX_DLL int NliCluster(const char* jsondir, int numfeatures, float* imgfeatures, tiorb_nli_cluster_info* cluster_info, int numshow=30, float thresh=0.85f);
EX_DLL int NliDestroy(tiorb_nli_cluster_info* cluster_info);

struct tiorb_nli_search_info { 
    int numshow = 30;
    int* indexs = nullptr;
    float* scores = nullptr;
};
EX_DLL int SearchImg(float* target, int numfeats, float* imgfeats, tiorb_nli_search_info* search_info, float thresh, int numshow=30);
EX_DLL int SearchDestroy(tiorb_nli_search_info* search_info);

struct tiorb_nli_scene_info { 
    int numshow = 0;
    int* indexs = nullptr;
};
EX_DLL int SceneInfer(float* target, int numFeats, float* imgFeats, tiorb_nli_scene_info* sceneInfo, float thresh);
EX_DLL int SceneDestroy(tiorb_nli_scene_info* scene_info);

struct tiorb_nli_hdf5_info { 
    int numshow = 0;
    char* content = nullptr;
    float* scores = nullptr;
};
EX_DLL struct Handle *GetHdf5FeatHandle();
EX_DLL int Hdf5FeatInit(Handle *handle, const char* hdf5_file, const char* datasetName);
EX_DLL int Hdf5FeatSearch(Handle *handle, float* target, tiorb_nli_hdf5_info* hdf5_info, float thresh);
//EX_DLL int Hdf5RetriveScene(Handle *handle, float* target, tiorb_nli_hdf5_info* hdf5_info, float thresh);
EX_DLL int Hdf5FeatDestroyStruct(tiorb_nli_hdf5_info* hdf5_info);
EX_DLL int Hdf5FeatDestroyHandle(Handle *handle);
}
