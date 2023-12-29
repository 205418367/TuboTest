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
    int type = atoi(argv[1]);        // 0:txt 1:img
    const char *model_dir = argv[2]; 
    const char *input_txt = argv[3]; 
    const char *input_img = argv[3]; 
    float thresh = atof(argv[4]);
    const char *output_dir= argv[5]; 
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    if (ret != 0) {
        std::cerr << "Failed to Init Params: "<< ret << std::endl;
        exit(1);
    } 
    std::ifstream jsonfile(utils::JoinPaths(model_dir,"imgfeat.json"));
    if (!jsonfile.good()) {
        std::cerr << "Failed to open jsonfile" << std::endl;
        exit(1);
    }
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
        //ret = NliTxtInit(handle, model_dir, 0);
	ret = NliENTxtInit(handle, model_dir, 0);
        if (ret != 0) {
            std::cerr << "Failed to NliENTxtInit: "<< ret << std::endl;
            exit(1);
        } 

	tiorb_txt_feat_info feat;
        ret = NliTxtInfer(handle, input_txt, &feat);
        if (ret != 0) {
            std::cerr << "Failed to NliTxtInfer: "<< ret << std::endl;
            exit(1);
        } 
	SearchImg(feat.txtFeature, numimgs, imgfeats, &search_info, 10);
	NliTxtDestroyStruct(&feat);
	NliTxtDestroy(handle);
    }else{
	struct Handle *handle = GetNliImgHandle(); 
        //ret = NliImgInit(handle, model_dir, 0);
	ret = NliENImgInit(handle, model_dir, 0);
        if (ret != 0) {
            std::cerr << "Failed to NliENImgInit: "<< ret << std::endl;
            exit(1);
        }

	tiorb_img_feat_info feat;
        ret = NliImgInferPath(handle, input_img, &feat);
        if (ret != 0) {
            std::cerr << "Failed to NliImgInfer: "<< ret << std::endl;
            exit(1);
        }
	SearchImg(feat.imgFeature, numimgs, imgfeats, &search_info, 10);
	NliImgDestroyStruct(&feat);
	NliImgDestroy(handle);
    }
    delete[] imgfeats;
        
    int numshow = search_info.numshow;
    int* indexs = search_info.indexs;
    float* scores=search_info.scores;
    for (int i=0; i<numshow; i++){
	std::string imgpath = pathvec[indexs[i]];
	if (scores[i] >= thresh){
	    std::cout<<"====>name:"<<imgpath<<" score:"<<scores[i]<<std::endl;
            std::string target = utils::JoinPaths(output_dir, std::to_string(i)+".jpg");
	    utils::CopyFile(imgpath, target);
	}
    }
    SearchDestroy(&search_info);
    return 0;
}

