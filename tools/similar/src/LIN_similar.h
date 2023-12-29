#pragma once
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

extern "C" {

struct TiorbModuleJsonInfo 
{
    char* content = nullptr;
};
EX_DLL int DestroyModuleJson(TiorbModuleJsonInfo *JsonInfo);

struct tiorb_similar_infer_info
{
    float* similarFeat = nullptr;
};
EX_DLL struct Handle* GetSimilarHandle(void);
EX_DLL int SimilarInit(Handle* handle, const char* modelDir, int forwardType);
EX_DLL int SimilarDestroy(Handle* handle);
EX_DLL int SimilarInferPath(Handle *handle, const char *imgpath, tiorb_similar_infer_info *infer_info);
EX_DLL int SimilarInfer(Handle *handle, const unsigned char *src, int size, tiorb_similar_infer_info *infer_info);
EX_DLL int SimilarDestroyStruct(tiorb_similar_infer_info *infer_info);
EX_DLL int SimilarInferJson(Handle* handle, const char* JsonStr, TiorbModuleJsonInfo* JsonInfo, int NumThreads);
}


