#pragma once
#ifndef _TIORB_SIMILAR_IMAGE_CLUSTER_H_
#define _TIORB_SIMILAR_IMAGE_CLUSTER_H_
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

extern "C" {

struct tiorb_similar_cluster_info {
    int num_clusters = 0; 
    int* labels = nullptr;
};
EX_DLL int SimilarCluster(int numfeatures, float* imgfeatures, tiorb_similar_cluster_info* cluster_info, float thresh=0.94f);
EX_DLL int SimilarDestroy(tiorb_similar_cluster_info* cluster_info);

}
#endif
