#include <iostream>
#include <vector>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "LIN_similar.h"
#include "LIN_simicluster.h"
#include "utils.h"
#include "linux_api.h"


int main(int argc, char **argv){
    const char *model_dir = argv[1]; //"models/";
    const char *input_dir = argv[2]; //"data/cluster";
    const char *image_ret = argv[3]; //"data/results";
    float thresh = atof(argv[4]);
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    printf("####InitParams_res %d \n", ret); 
    struct Handle *handle = GetSimilarHandle(); 
    ret = SimiInit(handle, model_dir, 0);
    if (ret != 0) printf("************* ClipImgInit failed! *************");
 

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
	    std::tuple<unsigned char*, int> result = utils::readBuffer(path);
            unsigned char* img = std::get<0>(result);
            int size = std::get<1>(result);

            tiorb_similar_infer_info feature;
            ret = SimiInfer(handle, img, size, &feature);
	    delete[] img;
            if (ret!=0) continue;

	    std::vector<float> feat=std::vector<float>(feature.similarFeat,feature.similarFeat+576);
	    SimiDestroyStruct(&feature);
            descriptors.push_back(feat);
	    pathvec.push_back(dir_entry->d_name);
	}
    }
    SimiDestroy(handle);

    std::cout<<"************************* cluster begin ************************* "<<std::endl;
    int num_feats = pathvec.size();
    float *imgfeatures = new float[num_feats*576];
    for (int i=0; i<num_feats; i++)
    {
	std::vector<float> imgfeat = descriptors[i];
	memcpy(&imgfeatures[i*576], &imgfeat[0], 576*sizeof(float));
    }
    tiorb_similar_cluster_info cluster_info;
    ret = SimilarCluster(num_feats, imgfeatures, &cluster_info, thresh);
    delete[] imgfeatures;

    int num = cluster_info.num_clusters;
    int* labels = cluster_info.labels;
    for (int i = 0; i<num_feats; i++) {
	int index = labels[i];
        if (index == -1) continue;
	std::string image_dir = std::string(image_ret)+"/"+ std::to_string(index);
	mkdir(image_dir.c_str(), 0755);
        std::string file_path = std::string(input_dir) +"/"+ pathvec[i];
	utils::copyToDir(file_path.c_str(), (image_dir+"/"+pathvec[i]).c_str());
    }
    SimilarDestroy(&cluster_info);
    std::cout<<"************************* cluster end ************************* "<<std::endl;
    return 0;
}

