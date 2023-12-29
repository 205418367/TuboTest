#include "linux_api.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string.h>
#include "LIN_nliimage.h"
#include "LIN_nlicluster.h"
#include "utils.h"
#include "json.h"
#include <chrono>

using json = nlohmann::json;

int main(int argc, char **argv){
    const char *model_dir = argv[1]; //"models/";
    const char *input_dir = argv[2]; //"data/cluster";
    const char *image_dir = argv[3]; //"data/results";
    int showid = atoi(argv[4]);
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2", "");
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
 
    std::vector<std::string> pathvec;
    std::vector<std::vector<float>> descriptors;
    vector<string> TraverseVec = utils::TraverseDirectory(input_dir);
    for (auto path : TraverseVec){
	    std::cout<< path <<std::endl;
            tiorb_img_feat_info feat;
	    const auto &start1 = std::chrono::steady_clock::now();
	    ret = NliImgInferPath(handle, path.c_str(), &feat);
	    const auto &end1 = std::chrono::steady_clock::now();
	    std::cout<<"#### infer time:"<<(end1 - start1).count() / 1000000.0 <<"ms"<<std::endl;
	    if (ret != 0) {
                std::cerr << "Failed to NliImgInfer: "<< ret << std::endl;
                continue;
            }
	    
            descriptors.push_back(std::vector<float>(feat.imgFeature,feat.imgFeature+1024));
	    pathvec.push_back(path);
	    NliImgDestroyStruct(&feat);
    }
    NliImgDestroy(handle);

    std::cout<<"************************* cluster begin ************************* "<<std::endl;
    int num_feats = pathvec.size();
    float *imgfeatures = new float[num_feats*1024];
    for (int i=0; i<num_feats; i++)
    {
	std::vector<float> imgfeat = descriptors[i];
	memcpy(&imgfeatures[i*1024], &imgfeat[0], 1024*sizeof(float));
    }
    tiorb_nli_cluster_info cluster_info;
    ret = NliCluster(model_dir, num_feats, imgfeatures, &cluster_info, showid);
    //ret = NliENCluster(model_dir, num_feats, imgfeatures, &cluster_info, showid);
    delete[] imgfeatures;

    json json_info = json::parse(cluster_info.content);
    std::cout<<"====>content:"<<cluster_info.content<<std::endl;

    int nums = json_info.size();
    for (int i=0; i<nums; i++){
	//std::string name = std::to_string(int(json_info[i]["name"]));
        std::string name = std::string(json_info[i]["name"]);
        auto indexVec = json_info[i]["index"];
	
	std::string imageDir = std::string(image_dir)+"/"+ std::to_string(i)+name;
	utils::CreateDirectory(imageDir);

	for (auto j : indexVec)
	{
	    std::string file_path = pathvec[int(j)];
	    string _, filename; utils::splitPathAndName(file_path, _, filename);   
	    utils::CopyFile(file_path, imageDir+"/"+filename);
	}
    }

    NliDestroy(&cluster_info);
    std::cout<<"************************* cluster end ************************* "<<std::endl;
    return 0;
}

