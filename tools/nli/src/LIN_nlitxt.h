#pragma once
#include <iostream>

extern "C" {
 
struct tiorb_txt_feat_info 
{
    float* txtFeature = nullptr; 
};

struct Handle *GetNliTxtHandle();

int NliTxtInit(Handle *handle, const char *modelDir, int forwardType);
int NliTxtDestroy(Handle *handle);
int NliTxtInfer(Handle *handle, const char *txt, tiorb_txt_feat_info *feature);
int NliTxtDestroyStruct(tiorb_txt_feat_info *feature);

}
