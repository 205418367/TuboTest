#pragma once

extern "C" {

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  

struct TiorbHumanSegmentInfo {
	unsigned char* img = nullptr;
        int img_size = 0;
};
EX_DLL struct Handle* HSegmentHandle();

EX_DLL int HSegmentInit(Handle* handle, const char* ModelPath, int ForwardType=0);

EX_DLL int HSegmentInferBuffer(Handle* handle, const unsigned char* src, int size, const unsigned char* bgimg, int length, TiorbHumanSegmentInfo* segment_info);
EX_DLL int HSegmentInferPath(Handle* handle, const char* imgpath, const char* bgpath, TiorbHumanSegmentInfo* segment_info);

EX_DLL int HSegmentDestroyModel(Handle* handle);
EX_DLL int HSegmentDestroyStruct(TiorbHumanSegmentInfo* segment_info);

}
