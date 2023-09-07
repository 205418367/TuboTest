#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include "LIN_nliimage.h"
#include "utils.h"
#include "linux_api.h"
#include "json.h"

int main(int argc, char **argv){
    const char *model_dir = argv[1]; //"models/";
    const char *input_dir = argv[2]; //"data/cluster";
    const char *image_ret = argv[3]; //"data/results";
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    printf("####InitParams_res %d \n", ret); 
    struct Handle *handle = GetNliImgHandle(); 
    ret = NliImgInit(handle, model_dir, 0);
    if (ret != 0) printf("************* NliImgInit failed! *************");
 
    struct dirent *dir_entry;
    DIR *dir = opendir(input_dir);
    if (!dir) {
        printf("unable to open directory\n");
        exit(1);
    }
    
    int numimgs = 0;
    nlohmann::json info_json, tmp_json;
    info_json["image"]=tmp_json;
    std::ofstream write(std::string(image_ret)+"/imgfeat.json");
    while ((dir_entry = readdir(dir)) != 0) {
        if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")) {
	    char path[1000];
            path[0] = '\0';
            strcat(path, input_dir);
            strcat(path, "/");
            strcat(path, dir_entry->d_name);
	    std::cout<< dir_entry->d_name <<std::endl;
	    std::tuple<unsigned char*, int> result = utils::readBuffer(path);
            unsigned char* img = std::get<0>(result);
            int size = std::get<1>(result);

            tiorb_img_feat_info feature;
            ret = NliImgInfer(handle, img, size, &feature);
	    delete[] img;
            if (ret!=0) continue;

	    nlohmann::json infojson;
	    infojson["path"] = dir_entry->d_name;
	    infojson["feat"] = std::vector<float>(feature.imgFeature,feature.imgFeature+1024);
	    info_json["image"].push_back(infojson);
	    numimgs += 1;
	    NliImgDestroyStruct(&feature);
	}
    }
    info_json["numimgs"] = numimgs;
    write << info_json.dump();
    write.close();
    return 0;
}

