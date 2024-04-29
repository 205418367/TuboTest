#pragma once
#include <iostream>

extern "C" {

struct TiorbModuleJsonInfo 
{
    char* content = nullptr;
};
int DestroyModuleJson(TiorbModuleJsonInfo *JsonInfo); 

// infer image
struct tiorb_img_feat_info 
{
    float* imgFeature = nullptr; 
};

struct Handle *GetNliImgHandle();

int NliImgInit(Handle *handle, const char *modelDir, int forwardType);
int NliImgDestroy(Handle *handle);
int NliImgInfer(Handle *handle, const unsigned char *src, int size, tiorb_img_feat_info *feature);
int NliImgInferPath(Handle *handle, const char *imgpath, tiorb_img_feat_info *feature);
int NliImgDestroyStruct(tiorb_img_feat_info *feature);
int NliImgInferJson(Handle* handle, const char* JsonStr, TiorbModuleJsonInfo* JsonInfo, int NumThreads);
}

