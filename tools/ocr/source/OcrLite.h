#ifndef _OCRLITE_H_
#define _OCRLITE_H_
#include <iostream>

extern "C" {

#if defined(_WIN32) || defined(_WIN64)
    #define EX_DLL __declspec(dllexport)
#else
    #define EX_DLL
#endif  


struct tiorb_img_ocr_info 
{
    char* content= nullptr;
};

EX_DLL struct Handle *GetOcrImgHandle();
EX_DLL int OcrImgInit(Handle *handle, const char *modelPath, int forwardType);
EX_DLL int OcrImgInferBuff(Handle *handle, const unsigned char *src, int size, tiorb_img_ocr_info *ocr_info);
EX_DLL int OcrImgInferPath(Handle *handle, const char *imgpath, tiorb_img_ocr_info *ocr_info);
EX_DLL int OcrImgDestroyStruct(tiorb_img_ocr_info *ocr_info);
EX_DLL int OcrImgDestroy(Handle *handle);

}
#endif
