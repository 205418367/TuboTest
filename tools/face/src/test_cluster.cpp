#include <vector>
#include <map>
#include <algorithm>
#include "utils.h"
#include "linux_api.h"
#include "smart_album_face_cluster.h"
#include "json.h"
using json = nlohmann::json;

int main(int argc, char **argv){
    int thread_nums =atoi(argv[1]);
    const char* model_dir = argv[2];
    const char* input_dir = argv[3];
    const char* output_dir= argv[4];
    const char* photo_dir= argv[5];
    float thresh = atof(argv[6]);

    int ret = InitParams(model_dir, "d2f73afa5d59cab5", "4523eac20d03df81c225d9260284f759");
    std::cout<< "====>Init Params:"<<ret<<std::endl;
    if (ret != 0) exit(1);
    Handle* handle = GetExtractHandle();
    ret = ExtractInit(handle, model_dir, 0, 0);
    if (ret != 0) {
        std::cerr << "Failed to Extract Init: "<< ret << std::endl;
        exit(1);
    }
    
    std::vector<std::string> TraverseDir = utils::TraverseDirectory(input_dir);
    json inJson(TraverseDir);
    TiorbModuleJsonInfo JsonInfo;
    TIME startTime = utils::GetCurrentTime();
    std::cout<<"====> thread_nums:"<<thread_nums<<std::endl;
    ret = ExtractImageJson(handle, input_dir, &JsonInfo, thread_nums, photo_dir);
    int64_t Duration = utils::GetDurationTime(startTime); 
    std::cout<<"====> thread_nums:"<<thread_nums<<" Duration:"<<Duration<<std::endl;
    if (ret != 0) {
        std::cerr << "Failed to Extract Image Json: "<< ret << std::endl;
        exit(1);
    }
    
    json outJson = json::parse(JsonInfo.content);
    std::vector<float> feat_vector;
    std::vector<std::string> file_vector;
    for (int i=0; i<outJson.size(); i++)
    {
        auto value = outJson[i];
        if ( value["result"] != 0) continue;
        if ( value["numface"]== 0) continue;
        std::vector<float> feats_vec = value["feature"].get<std::vector<float>>();
        feat_vector.insert(feat_vector.end(), feats_vec.begin(), feats_vec.end());
        for(int j=0;j<value["numface"];j++)file_vector.push_back(TraverseDir[i]);
    }
    DestroyModuleJson(&JsonInfo);
    ExtractDestroyModel(handle);
    
    int total_nums = feat_vector.size()/512;
    float* clusterfeats = feat_vector.data();
    tiorb_face_cluster_info cluster_info;
    FaceCluster(total_nums, clusterfeats, &cluster_info, thresh);
    std::cout<<"====>FaceCluster end!"<<std::endl;
    
    for (int i = 0; i < total_nums; i++) {
        
        int label = *(cluster_info.labels + i);
        if (label == -1) continue;   
        string _, filename; utils::splitPathAndName(file_vector[i], _, filename);      
        std::string imagedir = utils::JoinPaths(output_dir,std::to_string(label));
        utils::CreateDirectory(imagedir);
        utils::CopyFile(file_vector[i], utils::JoinPaths(imagedir,filename));
    }
    ClusterDestroy(&cluster_info);
    return 0;
}







