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

struct tiorb_blur_infer_info
{
    float blurScore = 0.f;
};

EX_DLL struct Handle* GetBlurHandle(void);
EX_DLL int BlurInit(Handle* handle, const char* modelDir, int forwardType);
EX_DLL int BlurDestroy(Handle* handle);

EX_DLL int BlurInferJson(Handle* handle, const char* JsonStr, TiorbModuleJsonInfo* JsonInfo, int NumThreads);
EX_DLL int BlurInferPath(Handle *handle, const char *imgpath, tiorb_blur_infer_info *infer_info);
EX_DLL int BlurInfer(Handle *handle, const unsigned char *src, int size, tiorb_blur_infer_info *infer_info);

}


