#include "linux_api.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "LIN_nliimage.h"
#include "LIN_nlicluster.h"
#include "utils.h"
#include <chrono>

int main(int argc, char **argv){
    const char *model_dir = argv[1]; //"models/";
    const char *input_dir = argv[2]; //"data/cluster";
    const char *image_ret = argv[3]; //"data/results";
    int showid = atoi(argv[4]);
    float thresh = atof(argv[5]);
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2", "");
    std::cout<<"鉴权##: "<< ret <<std::endl;
    struct Handle *handle = GetNliImgHandle(); 
    ret = NliImgInit(handle, model_dir, 0);
    if (ret != 0) printf("************* NliImgInit failed! *************");
 
    struct dirent *dir_entry;
    DIR *dir = opendir(input_dir);
    if (!dir) {
        printf("unable to open directory\n");
        exit(1);
    }
    
    std::vector<std::string> pathvec;
    std::vector<std::vector<float>> descriptors;
    while ((dir_entry = readdir(dir)) != 0) {
        if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")) {
	    char path[1000];
            path[0] = '\0';
            strcat(path, input_dir);
            strcat(path, "/");
            strcat(path, dir_entry->d_name);
	    std::cout<< dir_entry->d_name <<std::endl;

            tiorb_img_feat_info feature;
	    const auto &start1 = std::chrono::steady_clock::now();
	    ret = NliImgInferPath(handle, path, &feature);
	    const auto &end1 = std::chrono::steady_clock::now();
	    std::cout<<"#### infer time:"<<(end1 - start1).count() / 1000000.0 <<"ms"<<std::endl;
	
	    if (ret!=0) std::cout<<"####NliImgFeaturePath: "<< ret <<std::endl;
            if (ret!=0) continue;
	    

	    std::vector<float> feat=std::vector<float>(feature.imgFeature,feature.imgFeature+1024);
            descriptors.push_back(feat);
	    pathvec.push_back(dir_entry->d_name);
	    NliImgDestroyStruct(&feature);
	}
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
    ret = NliCluster(model_dir, num_feats, imgfeatures, &cluster_info, showid, thresh);
    delete[] imgfeatures;

    //int num = cluster_info.num_clusters;
    int* labels = cluster_info.labels;
    int* indexs = cluster_info.indexs;
    float* scores = cluster_info.scores;

    for (int i = 0; i<num_feats; i++) {
	int label = labels[i];
        if (label == -1) continue;
	std::string image_dir = std::string(image_ret)+"/"+ std::to_string(label);
	image_dir += "#"+albumfeat[indexs[label]]+":"+std::to_string(scores[label]);
	mkdir(image_dir.c_str(), 0755);
        std::string file_path = std::string(input_dir) +"/"+ pathvec[i];
	utils::copyToDir(file_path.c_str(), (image_dir+"/"+pathvec[i]).c_str());
    }
    NliDestroy(&cluster_info);
    std::cout<<"************************* cluster end ************************* "<<std::endl;
    return 0;
}

