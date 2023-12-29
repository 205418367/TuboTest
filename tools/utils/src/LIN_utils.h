#pragma once
#include <iostream>
 

extern "C" {
    struct TiorbModuleJsonInfo 
    {
        char* content = nullptr;
    };
    int DestroyModuleJson(TiorbModuleJsonInfo *JsonInfo);

    // 1.Generate thumbnail
    struct tiorb_resize_image_info {
        unsigned char* img = nullptr;
        int imgsize = 0;
    };
    int ResizeImageBuffer(const unsigned char* buffer, int length, int target, tiorb_resize_image_info* image_info);
    int ResizeDestroy(tiorb_resize_image_info* image_info);
    int ResizeImagePath(const char* imgpath, int target, const char* outpath);
    int ResizeImageBatch(const char* JsonStr,int target, const char* outdir, TiorbModuleJsonInfo* JsonInfo, int NumThreads);

    // 2.Extracting frames from a video
    int FrameExtraction(const char* inpath, int target, const char* outpath, int numFrame=1);
}

