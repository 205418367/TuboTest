#pragma once
#include <iostream>

extern "C" {

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

// infer image
struct tiorb_img_feat_info 
{
    float* imgFeature = nullptr; 
};

EX_DLL struct Handle *GetNliImgHandle();
EX_DLL int NliImgInit(Handle *handle, const char *modelPath, int forwardType);
EX_DLL int NliImgDestroy(Handle *handle);
EX_DLL int NliImgInfer(Handle *handle, const unsigned char *src, int size, tiorb_img_feat_info *feature);
EX_DLL int NliImgInferPath(Handle *handle, const char *imgpath, tiorb_img_feat_info *feature);
EX_DLL int NliImgDestroyStruct(tiorb_img_feat_info *feature);

}

