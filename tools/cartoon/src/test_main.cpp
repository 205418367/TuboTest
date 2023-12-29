#include "LIN_Cartoon.h"
#include "utils.h"
#include "linux_api.h"
#include <time.h>
#include <vector>
#include <chrono>


int main(int argc, char **argv) {
    int style = atoi(argv[1]);
    const char* model_dir = argv[2];
    const char* image_dir = argv[3];
    const char* outdr_dir = argv[4];

    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    if (ret != 0) std::cout<<"====>InitParams:"<<ret<<std::endl;

    Handle* handle = GetCartoonHandle();
    ret = CartoonInit(handle, model_dir, 0, style);
    if (ret != 0) std::cout<<"====>CartoonInit:"<<ret<<std::endl;

    vector<string> TraverseVec = utils::TraverseDirectory(image_dir);
    for (auto path : TraverseVec){
	std::cout<< path <<std::endl;
        TiorbImageCartoonInfo cartoon_info;
        ret = CartoonInferPath(handle, path.c_str(), &cartoon_info, outdr_dir, 1.5f);
        if (ret != 0) std::cout<<"====>CartoonInfer:"<<ret<<std::endl;
	std::cout<<"content:"<<cartoon_info.content<<std::endl;
	CartoonDestroyStruct(&cartoon_info);
    }
    CartoonDestroyModel(handle);
    return 0;
}
