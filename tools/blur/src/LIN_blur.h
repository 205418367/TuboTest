#pragma once
#ifndef Tiorb_Blur_Detect_H_
#define Tiorb_Blur_Detect_H_
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

extern "C" {

struct tiorb_blur_detect_info
{
    float blurScore[1];
};

EX_DLL struct Handle* GetBlurHandle(void);
EX_DLL int BlurInit(Handle* handle, const char* modelPath, int forwardType);
EX_DLL int BlurDetect(Handle *handle, const unsigned char *src, int size, tiorb_blur_detect_info *detect_info);
EX_DLL int BlurDestroy(Handle* handle);

}
#endif

