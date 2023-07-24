#pragma once
#ifndef Tiorb_CLIP_IMAGE_H_
#define Tiorb_CLIP_IMAGE_H_
#include <iostream>

extern "C" {

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

struct tiorb_img_feature_info {
    float imgFeature[1024]; 
};

// infer image
EX_DLL struct Handle *GetNliImgHandle();
EX_DLL int NliImgInit(Handle *handle, const char *modelPath, int forwardType);
EX_DLL int NliImgFeature(Handle *handle, const unsigned char *src, int size, tiorb_img_feature_info *feature);
EX_DLL int NliImgFeaturePath(Handle *handle, const char *imgpath, tiorb_img_feature_info *feature);
EX_DLL int NliImgDestroy(Handle *handle);

}
#endif
