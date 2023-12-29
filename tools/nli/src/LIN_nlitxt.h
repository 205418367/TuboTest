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

struct tiorb_txt_feat_info 
{
    float* txtFeature = nullptr; 
};

// infer txt
EX_DLL struct Handle *GetNliTxtHandle();
// english
EX_DLL int NliENTxtInit(Handle *handle, const char *modelDir, int forwardType);
// chinese 
EX_DLL int NliTxtInit(Handle *handle, const char *modelDir, int forwardType);
EX_DLL int NliTxtDestroy(Handle *handle);
EX_DLL int NliTxtInfer(Handle *handle, const char *txt, tiorb_txt_feat_info *feature);
EX_DLL int NliTxtDestroyStruct(tiorb_txt_feat_info *feature);

}
#endif
