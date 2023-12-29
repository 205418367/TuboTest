#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include "LIN_nliimage.h"
#include "utils.h"
#include "linux_api.h"
#include "json.h"

int main(int argc, char **argv){
    const char *model_dir = argv[1]; 
    const char *input_dir = argv[2]; 
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    if (ret != 0) {
        std::cerr << "Failed to Init Params: "<< ret << std::endl;
        exit(1);
    } 
    struct Handle *handle = GetNliImgHandle(); 
    ret = NliImgInit(handle, model_dir, 0);
    //ret = NliENImgInit(handle, model_dir, 0);
    if (ret != 0) {
        std::cerr << "Failed to NliENImgInit: "<< ret << std::endl;
        exit(1);
    } 
 
    int numimgs = 0;
    nlohmann::json info_json, tmp_json;
    info_json["image"]=tmp_json;
    std::ofstream write(utils::JoinPaths(model_dir,"imgfeat.json"));
    std::vector<std::string> TraverseVec = utils::TraverseDirectory(input_dir);
    for (auto path : TraverseVec){
        std::cout<<"====>"<<path<<std::endl;
        tiorb_img_feat_info feat;
        ret = NliImgInferPath(handle, path.c_str(), &feat);
        if (ret != 0) {
            std::cerr << "Failed to NliImgInfer: "<< ret << std::endl;
            continue;
        }
        nlohmann::json infojson;
	infojson["path"] = path;
	infojson["feat"] = std::vector<float>(feat.imgFeature,feat.imgFeature+1024);
	info_json["image"].push_back(infojson);
	numimgs += 1;
	NliImgDestroyStruct(&feat);
    }
    info_json["numimgs"] = numimgs;
    write << info_json.dump();
    write.close();
    return 0;
}

