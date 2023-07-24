#pragma once
#ifndef Tiorb_Scene_Detect_H_
#define Tiorb_Scene_Detect_H_
#include <iostream>
extern "C" {

struct tiorb_scene_detect_info {
  int num_scenes = 0;
  int *index = nullptr;
  float *accuracy = nullptr;
};
struct Handle* GetSceneHandle();

int SceneInit(Handle* handle, const char *scene_model_path, int Forwardtype);
int SceneDestroy(Handle* handle,tiorb_scene_detect_info *scene_info);
int SceneDestroyStruct(tiorb_scene_detect_info *detect_info);
int SceneDestroyModel(Handle *handle);
int SceneDetect(Handle* handle,const unsigned char *src, int size, tiorb_scene_detect_info *scene_info);

int VideoPathDetect(Handle* handle,const char *src_path, float thresh, tiorb_scene_detect_info *scene_info);
int VideoBufferDetect(Handle* handle,const unsigned char *src, int size, float thresh, tiorb_scene_detect_info *scene_info);
int get_latitude_longitude_info(std::string picture_file_path, std::string &lat, std::string &lng);
}
#endif
