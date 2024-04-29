#pragma once
#include "common.h"

extern "C" {
 
    /* ======================================================= imgMerge ======================================================= */
    struct tiorb_img_merge_info 
    {
        char* content= nullptr;
    };

    EX_DLL int MergeFromPath(int width,int height,const char* jsonname,const char* outdir,tiorb_img_merge_info* info);
    EX_DLL int MergeFromJson(int width,int height,const char* jsoninfo,const char* outdir,tiorb_img_merge_info* info);
    EX_DLL int MergeDestroy(tiorb_img_merge_info* info);
    /* ======================================================= imgMerge ======================================================= */
    

    /* ======================================================= appearChange ======================================================= */
    struct TiorbVideoAppearInfo 
    {
        char* content= nullptr;
    };
    int AppearChangeInit(const char* modelDir, int forwordType);
    int AppearChangeDestroy(TiorbVideoAppearInfo *info);
    int AppearChangeThread(const char* jsonInfo, const char* outDir, TiorbVideoAppearInfo* info, int frameRate=30, int numThreads=2);
    /* ======================================================= appearChange ======================================================= */
}

