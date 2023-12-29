#pragma once
#include <iostream>

extern "C" {

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  


struct tiorb_img_ocr_info 
{
    char* content = nullptr;
};

EX_DLL struct Handle *GetOcrHandle();
EX_DLL int OcrInit(Handle *handle, const char *modelPath, int forwardType);
EX_DLL int OcrInferBuffer(Handle *handle, const unsigned char *src, int size, tiorb_img_ocr_info *ocr_info);
EX_DLL int OcrInferPath(Handle *handle, const char *imgpath, tiorb_img_ocr_info *ocr_info);
EX_DLL int OcrDestroyStruct(tiorb_img_ocr_info *ocr_info);
EX_DLL int OcrDestroy(Handle *handle);

}

