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
    float thread = atof(argv[4]);
    float thresh = atof(argv[5]);
   
    int ret = InitParams(model_dir, "d2f73afa5d59cab5", "4523eac20d03df81c225d9260284f759");
    if (ret != 0) {
        printf("InitParams failed! \n");
        exit(1);
    }
    
    struct Handle *handle = GetSimilarHandle(); 
    ret = SimilarInit(handle, model_dir, thread);
    if (ret != 0) {
        printf("SimilarInit failed! \n");
        exit(1);
    }
    
    std::vector<std::string> pathvec;
    std::vector<std::vector<float>> descriptors;
    std::vector<std::string> TraverseDir = utils::TraverseDirectory(input_dir);
    for (auto path : TraverseDir){
	std::cout<< path <<std::endl;
        tiorb_similar_infer_info feature;
        ret = SimilarInferPath(handle, path.c_str(), &feature);
        if (ret!=0) continue;
	std::vector<float> feat=std::vector<float>(feature.similarFeat,feature.similarFeat+576);
	SimilarDestroyStruct(&feature);
        descriptors.push_back(feat);
	pathvec.push_back(path);
    }
    SimilarDestroy(handle);

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
        string directory, filename;
        utils::splitPathAndName(pathvec[i], directory, filename);
        utils::CopyFile(pathvec[i].c_str(), (image_dir+"/"+filename).c_str());
    }
    SimilarClusterDestroyStruct(&cluster_info);
    std::cout<<"************************* cluster end ************************* "<<std::endl;
    return 0;
}

