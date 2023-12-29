#pragma once

extern "C" {

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

struct TiorbImageCartoonInfo {
    char* content = nullptr;
};

EX_DLL struct Handle* GetCartoonHandle();
EX_DLL int CartoonInit(Handle* handle, const char* ModelPath, int ForwardType=0, int style=0);
EX_DLL int CartoonInferBuffer(Handle* handle, const unsigned char* src, int size, TiorbImageCartoonInfo* info, const char* outDir, float scale=1.5f);
EX_DLL int CartoonInferPath(Handle* handle, const char* imgpath, TiorbImageCartoonInfo* info, const char* outDir, float scale=1.5f);
EX_DLL int CartoonDestroyModel(Handle* handle);
EX_DLL int CartoonDestroyStruct(TiorbImageCartoonInfo* cartoon_info);

}
