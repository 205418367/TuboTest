#pragma once
#include <iostream>
 

extern "C" {
    // 1.Generate thumbnail
    int ResizeImageBuffer(const unsigned char* buffer, int length, int target, const char* outpath);
    int ResizeImagePath(const char* imgpath, int target, const char* outpath);
    //int ResizeImageBatch(const char* JsonStr,int target, const char* outdir, TiorbModuleJsonInfo* JsonInfo, int NumThreads);

    // 2.Extracting frames from a video
    //int FrameExtraction(const char* inpath, int target, const char* outdir, TiorbModuleJsonInfo* info, int numFrame=1);
    //int FrameExtractionBatch(const char* JsonStr, int target, const char* outdir, TiorbModuleJsonInfo* JsonInfo, int NumThreads, int numFrame=1);

    // 3. cv rectangle
    int DrawRectangle(const char* imgpath, int* rect, const char* outpath);
}
