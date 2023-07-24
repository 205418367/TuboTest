#include "LIN_Cartoon.h"
#include "utils.h"
#include "linux_api.h"
#include <time.h>
#include <vector>
#include <chrono>


int main(int argc, char **argv) {
    const char* image_file = argv[1];
    const char* image_resu = argv[2];
    const char* param_file = argv[3];
    const char* model_file = argv[3];
    int InitParams_res = InitParams(param_file, "ca3f59c7636cca88", "8e6487c5e3425dd3031221d2ddb43328");
    std::cout<<"########## 鉴权 "<< InitParams_res<<std::endl;

    Handle* handle = GetCartoonHandle();
    tiorb_ImageCartoon_info cartoon_info;
    //tiorb_ImageTransfer_info transfer_info;
    //TransferInit(handle, model_file, 4, 3);
    //int init_res = CartoonInit(handle, model_file, 0); // 整张图片漫画风
    int init_res = PDFaceInit(handle, model_file, 3);  // 百度人脸漫画风
    //int init_res = ANFaceInit(handle, model_file, 0);  // Animegan2人脸漫画风
    //std::cout<<"####init_res: "<<init_res<<std::endl;

    std::tuple<unsigned char*, int> result = utils::readBuffer(image_file);
    unsigned char* inputimage = std::get<0>(result);
    int size = std::get<1>(result);

    const auto &timeProcess0 = std::chrono::steady_clock::now();
    //TransferInfer(handle, inputimage, size, &transfer_info);
    //CartoonInfer(handle, inputimage, size, &cartoon_info);  // 整张图片漫画风
    PDFaceInfer(handle, inputimage, size, &cartoon_info);  // 百度人脸漫画风
    //ANFaceInfer(handle, inputimage, size, &cartoon_info);  // Animegan2人脸漫画风
    //std::cout<<"####init_res: "<<init_res<<std::endl;
    const auto &timeProcess1 = std::chrono::steady_clock::now();
    printf("infer time = %.3lf [msec]\n", (timeProcess1 - timeProcess0).count() / 1000000.0);

    utils::saveImage(cartoon_info.img, cartoon_info.img_size, image_resu);
    delete[] inputimage;
    //TransferDestroy(handle, &transfer_info);
    //CartoonDestroy(handle, &cartoon_info); // 整张图片漫画风
    PDFaceDestroy(handle, &cartoon_info);  // 百度人脸漫画风
    //ANFaceDestroy(handle, &cartoon_info);  // Animegan2人脸漫画风
    return 0;
}
