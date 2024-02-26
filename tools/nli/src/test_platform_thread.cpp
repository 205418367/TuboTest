#include <vector>
#include <algorithm>
#include "utils.h"
#include "linux_api.h"
#include "LIN_nliimage.h"
#include "LIN_nlicluster.h"
#include "json.h"

using json = nlohmann::json;

int main(int argc, char **argv){
    int thread_nums = atoi(argv[1]);
    const char* model_dir = argv[2];
    const char* input_dir = argv[3];
    const char* output_dir= argv[4];

    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    if (ret != 0) {
        std::cerr << "Failed to Init Params: "<< ret << std::endl;
        exit(1);
    }
    
    std::string imgfeat = utils::JoinPaths(input_dir,"imgfeat.json");
    if (!utils::FileExist(imgfeat)){
        struct Handle *handle = GetNliImgHandle(); 
        ret = NliImgInit(handle, model_dir, 0);
        if (ret != 0) {
            std::cerr << "Failed to NliImgInit: "<< ret << std::endl;
            exit(1);
        }
        std::ofstream write(imgfeat);
        TiorbModuleJsonInfo outjson;
        TIME startTime = utils::GetCurrentTime();
        ret = NliImgInferJson(handle, input_dir, &outjson, thread_nums);
        if (ret != 0) {
            std::cerr << "Failed to NliImgInferJson: "<< ret << std::endl;
            exit(1);
        } 
        int64_t Duration = utils::GetDurationTime(startTime); 
        std::cout<<"====> thread_nums:"<<thread_nums<<" D`uration:"<<Duration<<std::endl;
        write << std::string(outjson.content);
        write.close();
        DestroyModuleJson(&outjson); 
        NliImgDestroy(handle);
    }
    
    std::vector<std::string> TraverseDir = utils::TraverseDirectory(input_dir);
    std::ifstream jsonfile(imgfeat);
    json outJson = json::parse(jsonfile);
    jsonfile.close();
    std::vector<float> feat_vector;
    std::vector<std::string> file_vector;
    for (int i=0; i<outJson.size(); i++)
    {
        auto value = outJson[i];
        if ( value["result"] != 0) continue;
        std::vector<float> feats_vec = value["feature"].get<std::vector<float>>();
        feat_vector.insert(feat_vector.end(), feats_vec.begin(), feats_vec.end());
        file_vector.push_back(TraverseDir[i]);
    }
    
    int num_feats = feat_vector.size()/1024;
    float* imgfeatures = feat_vector.data();
    tiorb_nli_cluster_info cluster_info;
    ret = NliCluster(model_dir, num_feats, imgfeatures, &cluster_info, 30);
    //ret = NliENCluster(model_dir, num_feats, imgfeatures, &cluster_info, showid);
    
    json json_info = json::parse(cluster_info.content);
    std::cout<<"====>content:"<<cluster_info.content<<std::endl;
    
    int nums = json_info.size();
    for (int i=0; i<nums; i++){
	//std::string name = std::to_string(int(json_info[i]["name"]));
        std::string name = std::string(json_info[i]["name"]);
        auto indexVec = json_info[i]["index"];
	
	std::string imagedir = utils::JoinPaths(output_dir,std::to_string(i)+name);
	utils::CreateDirectory(imagedir);
	for (auto j : indexVec)
	{
	    std::string file_path = file_vector[int(j)];
	    string _, filename; utils::splitPathAndName(file_path, _, filename);   
	    utils::CopyFile(file_path, imagedir+"/"+filename);
	}
    }
    NliDestroy(&cluster_info);
    return 0;
}







