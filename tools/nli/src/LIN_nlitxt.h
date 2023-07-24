#pragma once
#ifndef Tiorb_CLIP_TXT_H_
#define Tiorb_CLIP_TXT_H_
#include <iostream>

extern "C" {

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

struct tiorb_txt_feature_info {
    float txtFeature[1024]; 
};

// infer txt
EX_DLL struct Handle *GetNliTxtHandle();
EX_DLL int NliTxtInit(Handle *handle, const char *modelPath, int forwardType);
EX_DLL int NliTxtFeature(Handle *handle, const char *txt, tiorb_txt_feature_info *feature);
EX_DLL int NliTxtDestroy(Handle *handle);

}
#endif
