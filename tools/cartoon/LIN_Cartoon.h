#pragma once
#ifndef TIORB_CARTOON_H_
#define TIORB_CARTOON_H_

extern "C" {

struct tiorb_ImageCartoon_info {
	unsigned char* img = nullptr;
        int img_size = 0;
};

// 整张图片漫画风
struct Handle* GetCartoonHandle();
int CartoonInit(Handle* handle, const char* ModelPath, int ForwardType);
int CartoonInfer(Handle* handle, const unsigned char* src, int size, tiorb_ImageCartoon_info* cartoon_info);
int CartoonDestroyModel(Handle* handle);
int CartoonDestroy(Handle* handle, tiorb_ImageCartoon_info* cartoon_info);
int CartoonDestroyStruct(tiorb_ImageCartoon_info* cartoon_info);

// 百度人脸漫画风
struct Handle* GetPDFaceHandle();
int PDFaceInit(Handle* handle, const char* ModelPath, int ForwardType);
int PDFaceInfer(Handle* handle, const unsigned char* src, int size, tiorb_ImageCartoon_info* cartoon_info);
int PDFaceDestroyModel(Handle* handle);
int PDFaceDestroy(Handle* handle, tiorb_ImageCartoon_info* cartoon_info);
int PDFaceDestroyStruct(tiorb_ImageCartoon_info* cartoon_info);

// Animegan2人脸漫画风
struct Handle* GetANFaceHandle();
int ANFaceInit(Handle* handle, const char* ModelPath, int ForwardType);
int ANFaceDestroy(Handle* handle, tiorb_ImageCartoon_info* aface_info);
int ANFaceDestroyModel(Handle* handle);
int ANFaceDestroyStruct(tiorb_ImageCartoon_info* cartoon_info);
int ANFaceInfer(Handle* handle, const unsigned char* src, int size, tiorb_ImageCartoon_info* aface_info);

struct tiorb_ImageTransfer_info {
	unsigned char* img = nullptr;
        int img_size = 0;
};
//风格迁移
struct Handle* GetTransferHandle();
int TransferInit(Handle* handle, const char* ModelPath, int style, int ForwardType);
int TransferInfer(Handle* handle, const unsigned char* src, int size, tiorb_ImageTransfer_info* transfer_info);
int TransferDestroyModel(Handle* handle);
int TransferDestroy(Handle* handle, tiorb_ImageTransfer_info* transfer_info);
int TransferDestroyStruct(tiorb_ImageTransfer_info* transfer_info);
}
#endif
