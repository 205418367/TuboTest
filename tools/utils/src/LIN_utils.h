#pragma once
#include <iostream>
 

extern "C" {
    struct TiorbModuleJsonInfo 
    {
        char* content = nullptr;
    };
    int DestroyModuleJson(TiorbModuleJsonInfo* JsonInfo);
    
    // 1.Generate thumbnail
    int ResizeImageBuffer(const unsigned char* buffer, int length, int target, const char* outpath);
    int ResizeImagePath(const char* imgpath, int target, const char* outpath);

    // 2.Extracting frames from a video
    int FrameExtraction(const char* inpath, int target, const char* outdir, TiorbModuleJsonInfo* info, int numFrame=1);
    int FrameBufferExtract(const unsigned char* videoBuffer, int bufferSize, int target, const char* outdir, TiorbModuleJsonInfo* info, int numFrame);

    // 3. cv rectangle
    //EX_DLL int DrawRectangle(const char* imgpath, int* rect, const char* outpath);
    
    // 4. transferHeic2Jpg
    int transferHeic2Jpg(const char* heicPath, const char* outputPath, int qulity);
    
    int applyLutEffect(const char* cubePath, const char* picPath, const char* outPath, float value=1.f);
}
