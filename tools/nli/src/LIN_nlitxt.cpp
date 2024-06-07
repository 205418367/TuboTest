#pragma once
#include "LIN_nlitxt.h"

extern "C" {
 
int NliTxtInit(Handle *handle, const char *modelDir, int forwardType)
{

}

int NliTxtDestroy(Handle *handle)
{

}

int NliTxtInfer(Handle *handle, const char *txt, tiorb_txt_feat_info *feature)
{

}

int NliTxtDestroyStruct(tiorb_txt_feat_info *feature)
{

}

}
