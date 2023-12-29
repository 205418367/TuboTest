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
    const char *output_dir= argv[3];
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    if (ret != 0) {
        std::cerr << "Failed to Init Params: "<< ret << std::endl;
        exit(1);
    } 
    
    struct Handle *handle = GetBlurHandle(); 
    ret = BlurInit(handle, model_dir, 0);
    if (ret != 0) {
        std::cerr << "Failed to Blur Init: "<< ret << std::endl;
        exit(1);
    } 
 
    vector<string> TraverseVec = utils::TraverseDirectory(input_dir);
    for (auto path : TraverseVec){
	std::cout<< path <<std::endl;

        tiorb_blur_infer_info detect;
        ret = BlurInferPath(handle, path.c_str(), &detect);
        if (ret!=0) continue;
        
        float blur_score = detect.blurScore;
	string _, Name; utils::splitPathAndName(path, _, Name);
	if (blur_score > 50.f) {
	    utils::CopyFile(path, utils::JoinPaths(output_dir,"sharp/"+Name));
	}else if (blur_score <= 50.f && blur_score > 40.f){
	    utils::CopyFile(path, utils::JoinPaths(output_dir, "norm/"+Name));
        }else if (blur_score <= 40.f){
            utils::CopyFile(path, utils::JoinPaths(output_dir, "blur/"+Name));
        }
    }
    BlurDestroy(handle);
    return 0;
}

