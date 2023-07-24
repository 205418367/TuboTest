#pragma once
#ifndef _TIORB_UTILS_H_
#define _TIORB_UTILS_H_
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

extern "C" {

EX_DLL int FrameExtraction(const char* inpath, int target, const char* outpath);
EX_DLL int ResizeImagePath(const char* imgpath, int target, const char* outpath);
EX_DLL int transferHeic2Jpg(const char* heicPicPath,const char* outputPicPath,int compressQulity);
}
#endif

