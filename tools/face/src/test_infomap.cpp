#include "linux_api.h"
#include "smart_album_face_cluster.h"
#include "utils.h"
#include <iostream>
#include <cstring>

int main(int argc, char **argv) {
    int PREFaceNums = atoi(argv[1]);
    const char* imgname = argv[2]; 
    const char* model_dir = argv[3]; 

    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2", "");
    std::cout<<"InitParams ##: "<< ret<<std::endl;

    struct Handle* handle = GetExtractHandle();
    ret = ExtractFaceInit(handle, model_dir, 0, 0);
    if (ret==0){
        std::cout << "ExtractInit: " << ret << std::endl;
    }else{
        return -1;
    }

    std::tuple<unsigned char*, int> result = utils::readBuffer(imgname);
    unsigned char* inputimage = std::get<0>(result);
    int size = std::get<1>(result);

    tiorb_image_feats_info feats_info;
    ret = ExtractImageFeats(handle, inputimage, size, &feats_info, 1.5f, false);
    delete[] inputimage;
    if (ret==0){
        std::cout << "ExtractImageFeats: " << ret << std::endl;
    }else{
        return -1;
    }
    
    float* FaceFeature = feats_info.FaceFeature;
    float* clusterfeats = new float[PREFaceNums * 512];
    for (int i = 0; i < PREFaceNums; ++i) 
    {
	memcpy(clusterfeats+512*i, FaceFeature, sizeof(float)*512);
    }
    ExtractDestroyStruct(&feats_info);

    tiorb_face_cluster_info cluster_info;
    for (int i=0; i<10000; i++) {
	FaceCluster(PREFaceNums, clusterfeats, &cluster_info, 0.53f);
	ClusterDestroy(&cluster_info);
	std::cout<<"FaceCluster ========================== " + std::to_string(i)<<std::endl;
    }
    return 0;
}







