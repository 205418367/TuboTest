#pragma once

extern "C" {

struct tiorb_image_feats_info {
        int num_face = 0;            
        int* location = nullptr;     
        float* quality = nullptr;     
	float* FaceFeature = nullptr; 
};
struct Handle* GetExtractHandle();  

int ExtractFaceInit(Handle* handle, const char* ModelPath, int ForwardType, int ModelType, int InputSize=320);
int ExtractInit(Handle* handle, const char* ModelPath, int ForwardType, int ModelType );
int ExtractImageFeats(Handle* handle, const unsigned char* src, int size, tiorb_image_feats_info* feats_info, const char* imgdir=NULL);
int ExtractImagePath(Handle* handle, const char* srcpath, tiorb_image_feats_info* feats_info, const char* imgdir=NULL);
int ExtractDestroyModel(Handle* handle);
int ExtractDestroy(Handle* handle, tiorb_image_feats_info* feats_info);
int ExtractDestroyStruct(tiorb_image_feats_info* feats_info);


//************************************** 人脸画框 **************************************//
int DrawRectangle(const char* imgpath, int* rect, const char* outpath);

//************************************** 多线程 **************************************//
struct TiorbModuleJsonInfo 
{
    char* content = nullptr;
};
int DestroyModuleJson(TiorbModuleJsonInfo *JsonInfo);
int ExtractImageJson(Handle* handle, const char* JsonStr, TiorbModuleJsonInfo* JsonInfo, int numThreads, const char* imgdir=NULL);

}  

