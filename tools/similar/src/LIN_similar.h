#pragma once
#ifndef TIORB_SIMILAR_DETECT_H_
#define TIORB_SIMILAR_DETECT_H_
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

extern "C" {

EX_DLL struct Handle* GetSimilarHandle(void);
EX_DLL int SimiInit(Handle* handle, const char* modelPath, int forwardType);
EX_DLL int SimiDestroy(Handle* handle);

struct tiorb_similar_detect_info
{
    float similarFeat[576];
};
EX_DLL int SimiDetectPath(Handle *handle, const char *imgpath, tiorb_similar_detect_info *detect_info);
EX_DLL int SimiDetect(Handle *handle, const unsigned char *src, int size, tiorb_similar_detect_info *detect_info);


struct tiorb_similar_infer_info
{
    float* similarFeat = nullptr;
};
EX_DLL int SimiInferPath(Handle *handle, const char *imgpath, tiorb_similar_infer_info *infer_info);
EX_DLL int SimiInfer(Handle *handle, const unsigned char *src, int size, tiorb_similar_infer_info *infer_info);
EX_DLL int SimiDestroyStruct(tiorb_similar_infer_info *infer_info);

}
#endif

