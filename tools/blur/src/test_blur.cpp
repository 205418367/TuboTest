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
    printf("####InitParams_res %d \n", ret); 
    struct Handle *handle = GetBlurHandle(); 
    ret = BlurInit(handle, model_dir, 0);
    if (ret != 0) printf("************* BlurInit failed! *************");
 
    struct dirent *dir_entry;
    DIR *dir = opendir(input_dir);
    if (!dir) {
        printf("unable to open directory\n");
        exit(1);
    }
    
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

            tiorb_blur_infer_info detect;
	    ret = BlurInfer(handle, img, size, &detect);
	    delete[] img;
            if (ret!=0) continue;
	    float blur_score = detect.blurScore;

	    std::string srcdir = std::string(input_dir)+"/"+dir_entry->d_name;
	    std::string detdir = std::string(image_ret)+"/";
	    if (blur_score > 50.f) {
		utils::copyToDir(srcdir, detdir+"sharp/"+std::string(dir_entry->d_name));
	    }else if (blur_score < 40.f){
		utils::copyToDir(srcdir, detdir+"blur/"+std::string(dir_entry->d_name));
            }
	}
    }
    BlurDestroy(handle);
    return 0;
}

