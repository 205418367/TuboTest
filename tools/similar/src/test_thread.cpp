#include <vector>
#include <algorithm>
#include "utils.h"
#include "linux_api.h"
#include "LIN_similar.h"
#include "LIN_simicluster.h"
#include "json.h"
using json = nlohmann::json;

int main(int argc, char **argv){
    int thread_nums =atoi(argv[1]);
    const char* model_dir = argv[2];
    const char* input_dir = argv[3];
    const char* output_dir= argv[4];
    int forward_type =atoi(argv[5]);

    int ret = InitParams(model_dir, "d2f73afa5d59cab5", "4523eac20d03df81c225d9260284f759");
    std::cout<< "====> Init Params:"<<ret<<std::endl;
    if (ret != 0) exit(1);
    
    Handle* handle = GetSimilarHandle();
    ret = SimilarInit(handle, model_dir, forward_type);
    if (ret != 0) {
        std::cerr << "Failed to Extract Init: "<< ret << std::endl;
        exit(1);
    }
    
    std::vector<std::string> TraverseDir = utils::TraverseDirectory(input_dir);
    
    TiorbModuleJsonInfo JsonInfo;
    TIME startTime = utils::GetCurrentTime();
    ret = SimilarInferJson(handle, input_dir, &JsonInfo, thread_nums);
    int64_t Duration = utils::GetDurationTime(startTime); 
    std::cout<<"====> thread_nums:"<<thread_nums<<" Duration:"<<Duration<<std::endl;
    
    if (ret != 0) {
        std::cerr << "Failed to Extract Image Json: "<< ret << std::endl;
        exit(1);
    }
    
    std::vector<float> feat_vector;
    std::vector<std::string> file_vector;
    json outJson = json::parse(JsonInfo.content);
    for (int i=0; i<outJson.size(); i++)
    {
        auto value = outJson[i];
        if ( value["result"] != 0) continue;
        std::vector<float> feats_vec = value["feature"].get<std::vector<float>>();
        feat_vector.insert(feat_vector.end(), feats_vec.begin(), feats_vec.end());
        file_vector.push_back(TraverseDir[i]);
    }
    DestroyModuleJson(&JsonInfo);
    SimilarDestroy(handle);
    
    int num_feats = feat_vector.size()/576;
    float* clusterfeats = feat_vector.data();
    tiorb_similar_cluster_info cluster_info;
    ret = SimilarCluster(num_feats, clusterfeats, &cluster_info, 0.94f);
    for (int i = 0; i<num_feats; i++) {
	int label = *(cluster_info.labels + i);
        if (label == -1) continue;   
        string _, filename; utils::splitPathAndName(file_vector[i], _, filename);      
        std::string imagedir = utils::JoinPaths(output_dir,std::to_string(label));
        utils::CreateDirectory(imagedir);
        utils::CopyFile(file_vector[i], utils::JoinPaths(imagedir,filename));
    }
    SimilarClusterDestroyStruct(&cluster_info);
    return 0;
}







