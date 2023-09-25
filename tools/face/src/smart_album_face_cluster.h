#pragma once
#ifndef _FACE_RECOGNIZER_H_
#define _FACE_RECOGNIZER_H_

extern "C" {

// 可定义全局变量也可定义局部变量
// 如果定义为全局变量, 必须使用ExtractDestroy释放
// 如果定义为局部变量, 必须使用ExtractDestroyStruct与ExtractDestroyModel释放
struct tiorb_image_feats_info {
        int num_face = 0;             // 检测到的人脸数量
        int* location = nullptr;      // 人脸坐标
        float* quality = nullptr;     // 人脸质量分
        unsigned char* img = nullptr; // 封面图片
        int* img_size = nullptr;      // 封面图片大小
	float* FaceFeature = nullptr; // 人脸特征
};
struct Handle* GetExtractHandle();  // 获取handle  

int ExtractInit(Handle* handle, const char* ModelPath, int ForwardType, int ModelType );
int ExtractFaceInit(Handle* handle, const char* ModelPath, int ForwardType, int ModelType, int InputSize=640);
// scale 人物所占封面比例, 建议设置1.5f; rotation 多角度检测人脸, 默认false
int ExtractImageFeats(Handle* handle, const unsigned char* src, int size, tiorb_image_feats_info* feats_info, float scale=1.5, bool rotation=false);
// 释放模型
int ExtractDestroyModel(Handle* handle);
// 释放模型和结构体
int ExtractDestroy(Handle* handle, tiorb_image_feats_info* feats_info);
// 释放结构体
int ExtractDestroyStruct(tiorb_image_feats_info* feats_info);


//************************************** 人脸检测 1 **************************************//  
struct tiorb_face_detect_info {
	int num_faces = 0;
	int* location = nullptr;
};
struct Handle* GetFaceHandle();

int ImageFaceInit(Handle* handle, const char* FaceDetectModelPath, int ForwardType);
int ImageFaceDestroy(Handle* handle, tiorb_face_detect_info* face_info);
int ImageDestroyStruct(tiorb_face_detect_info* face_info);
int ImageDestroyModel(Handle* handle);
int ImageFaceDetect(Handle* handle, const unsigned char* src, int size, tiorb_face_detect_info* face_info);

//************************************** 人脸关键点 **************************************//
struct tiorb_face_photo_info {
        unsigned char* img = nullptr;
        int img_size = 0;
};
struct Handle* GetPhotoHandle();

int PhotoInit(Handle* handle, const char* ModelPath, int ForwardType);
int PhotoDestroyModel(Handle* handle);
int PhotoDestroyStruct(tiorb_face_photo_info* photo_info);
int PhotoExtract(Handle* handle, const unsigned char* src, int size, int* rect, tiorb_face_photo_info* photo_info, float scale=1.5f, bool rotation=false);

//************************************** 特征提取 **************************************//
struct tiorb_face_feature_info {
	float face_quality = 0.f; 
	float* FaceFeature = nullptr; // 512维
};
struct Handle* GetFeatureHandle();

int FeatureInit(Handle* handle, const char* LandmarkModelPath, int ForwardType, int modeltype);
int ExtractFeature(Handle* handle, const unsigned char* src, int size, tiorb_face_feature_info* feature_info);
int FeatureDestroyModel(Handle* handle);
int FeatureDestroy(Handle* handle, tiorb_face_feature_info* feature_info);
int FeatureDestroyStruct(tiorb_face_feature_info* feature_info);
}
#endif  

