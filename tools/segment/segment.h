#pragma once
#ifndef LIN_SEGMENT_H_
#define LIN_SEGMENT_H_

extern "C" {

struct tiorb_SkySegment_info {
	unsigned char* img = nullptr;
        int img_size = 0;
};
struct Handle* SKYSegmentHandle();
int SkySegmentInit(Handle* handle, const char* ModelPath, int ForwardType);
int SkySegmentInfer(Handle* handle, const unsigned char* src, int size, const unsigned char* bgimg, int length, tiorb_SkySegment_info* sky_info);
int SkySegmentDestroyModel(Handle* handle);
int SkySegmentDestroy(Handle* handle, tiorb_SkySegment_info* sky_info);
int SkySegmentDestroyStruct(tiorb_SkySegment_info* sky_info);

struct tiorb_PersonSegment_info {
	unsigned char* img = nullptr;
        int img_size = 0;
};
// 魔法背景
struct Handle* MODSegmentHandle();
int ISegmentInit(Handle* handle, const char* ModelPath, int ForwardType=0);
int ISegmentInfer(Handle* handle, const unsigned char* src, int size, const unsigned char* bgimg, int length, tiorb_PersonSegment_info* segment_info);
int ISegmentDestroyModel(Handle* handle);
int ISegmentDestroy(Handle* handle, tiorb_PersonSegment_info* segment_info);
int ISegmentDestroyStruct(tiorb_PersonSegment_info* segment_info);

}
#endif
