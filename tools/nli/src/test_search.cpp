#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include "LIN_nlicluster.h"
#include "LIN_nliimage.h"
#include "LIN_nlitxt.h"
#include "utils.h"
#include "linux_api.h"
#include "json.h"

int main(int argc, char **argv){
    int type = atoi(argv[1]); //0; // 0:txt 1:img
    const char *model_dir = argv[2]; //"models/";
    const char *json_file = argv[3]; //"data/json/imgfeat.json";
    const char *key_words = argv[4]; //"天空";
    const char *input_img = argv[4]; //"data/images/65.jpg";
    const char *input_dir = argv[5]; //"data/cluster/";
    const char *image_ret = argv[6]; //"data/results";
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    printf("####InitParams_res %d \n", ret); 
    std::ifstream jsonfile(json_file);
    nlohmann::json data = nlohmann::json::parse(jsonfile);
    int numimgs = data["numimgs"];
    float *imgfeats = new float[numimgs*1024];
    std::vector<std::string> pathvec(numimgs);
    for (int j=0; j<numimgs; j++){
	pathvec[j] = data["image"][j]["path"];
	std::vector<float> fVector = data["image"][j]["feat"].get<std::vector<float>>();
	memcpy(imgfeats+j*1024,fVector.data(),fVector.size()*sizeof(float));
    }  

    tiorb_nli_search_info search_info;
    if (type == 0){
	struct Handle *handle = GetNliTxtHandle(); 
        ret = NliTxtInit(handle, model_dir, 0);
        if (ret != 0) printf("************* NliTxtInit failed! ************* \n");

	tiorb_txt_feat_info  feat;
        ret = NliTxtInfer(handle, key_words, &feat);
        if (ret != 0) printf("************* NliTxtFeature failed! ************* \n");
	SearchImg(feat.txtFeature, numimgs, imgfeats, &search_info, 30);
	NliTxtDestroyStruct(&feat);
	NliTxtDestroy(handle);
    }else{
	struct Handle *handle = GetNliImgHandle(); 
        ret = NliImgInit(handle, model_dir, 0);
        if (ret != 0) printf("************* NliImgInit failed! ************* \n");

        std::tuple<unsigned char*, int> result = utils::readBuffer(input_img);
        unsigned char* img = std::get<0>(result);
        int size = std::get<1>(result);
	tiorb_img_feat_info feature;
        ret = NliImgInfer(handle, img, size, &feature);
        delete[] img;
        if (ret != 0) printf("************* NliImgFeature failed! ************* \n");
	SearchImg(feature.imgFeature, numimgs, imgfeats, &search_info, 1);
	NliImgDestroyStruct(&feature);
	NliImgDestroy(handle);
    }
    delete[] imgfeats;
        
    int numshow = search_info.numshow;
    int* indexs = search_info.indexs;
    float* scores=search_info.scores;
    for (int i=0; i<numshow; i++){
	std::string imagename = pathvec[indexs[i]];
        std::string srcimg = std::string(input_dir)+"/"+imagename;
        std::string tarimg = std::string(image_ret)+"/"+imagename;
	utils::CopyFile(srcimg, tarimg);
    }
    SearchDestroy(&search_info);
    return 0;
}

