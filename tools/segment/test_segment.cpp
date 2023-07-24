#include "linux_api.h"
#include "segment.h"
#include "utils.h"
#include <time.h>
#include <vector>

int main(int argc, char **argv) {
    const char* model_file = argv[3];
    const char* image_file = argv[1];
    const char* bgimg_file = argv[2];
    int InitParams_ret = InitParams(model_file, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    std::cout<<"########## 鉴权 "<< InitParams_ret<<std::endl;

    tiorb_SkySegment_info segment_info;
    Handle* handle = SKYSegmentHandle();
    SkySegmentInit(handle, model_file, 0);

    std::tuple<unsigned char*, int> result = utils::readBuffer(image_file);
    unsigned char* inputimage = std::get<0>(result);
    int size = std::get<1>(result);
    std::tuple<unsigned char*, int> result1 = utils::readBuffer(bgimg_file);
    unsigned char* bgimg = std::get<0>(result1);
    int length = std::get<1>(result1);

    clock_t startTime, endTime;
    startTime = clock();
    int ret = SkySegmentInfer(handle, inputimage, size, bgimg, length, &segment_info);
    endTime = clock();
    if (ret != 0) {
	printf("###### error code: %d \n", ret);
	return 0;
    }
    printf("segment totile time=%f ms\n",(float)(endTime-startTime)*1000/CLOCKS_PER_SEC);
    utils::saveImage(segment_info.img, segment_info.img_size, "result.jpg");
    delete[] inputimage;
    SkySegmentDestroy(handle, &segment_info);
    return 0;
}
