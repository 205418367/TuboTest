#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include "LIN_blur.h"
#include "utils.h"
#include "linux_api.h"
#include "json.h"

int main(int argc, char **argv){
    const char *model_dir = argv[1];
    const char *input_dir = argv[2];
    const char *image_ret = argv[3];
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    printf("####InitParams: %d \n", ret); 
    struct Handle *handle = GetBlurHandle(); 
    ret = BlurInit(handle, model_dir, 0);
    if (ret != 0) printf("************* BlurInit failed! *************");
 
    vector<string> TraverseVec = utils::TraverseDirectory(input_dir);
    for (auto path : TraverseVec){
	    std::cout<< path <<std::endl;
	    std::tuple<unsigned char*, int> result = utils::readBuffer(path.c_str());
            unsigned char* img = std::get<0>(result);
            int size = std::get<1>(result);

            tiorb_blur_infer_info detect;
	    ret = BlurInfer(handle, img, size, &detect);
	    delete[] img;
            if (ret!=0) continue;
	    float blur_score = detect.blurScore;

	    string directory, fileName;
	    utils::splitPathAndName(path, directory, fileName);
	    std::string srcdir = std::string(input_dir)+"/"+fileName;
	    std::string detdir = std::string(image_ret)+"/";
	    if (blur_score > 50.f) {
		utils::CopyFile(srcdir, detdir+"sharp/"+fileName);
	    }else if (blur_score < 40.f){
		utils::CopyFile(srcdir, detdir+"blur/"+fileName);
            }
    }
    BlurDestroy(handle);
    return 0;
}

