#include <vector>
#include <map>
#include <algorithm>
#include "utils.h"
#include "linux_api.h"
#include "LIN_blur.h"
#include "json.h"
using json = nlohmann::json;

int main(int argc, char **argv){
    int thread_nums =atoi(argv[1]);
    const char* model_dir = argv[2];
    const char* input_dir = argv[3];
    const char* output_dir= argv[4];

    int ret = InitParams(model_dir, "d2f73afa5d59cab5", "4523eac20d03df81c225d9260284f759");
    std::cout<< "====> Init Params:"<<ret<<std::endl;
    if (ret != 0) exit(1);
    
    struct Handle *handle = GetBlurHandle(); 
    ret = BlurInit(handle, model_dir, 0);
    if (ret != 0) {
        std::cerr << "Failed to Extract Init: "<< ret << std::endl;
        exit(1);
    }
    
    std::vector<std::string> TraverseDir = utils::TraverseDirectory(input_dir);
    
    TiorbModuleJsonInfo JsonInfo;
    TIME startTime = utils::GetCurrentTime();
    ret = BlurInferJson(handle, input_dir, &JsonInfo, thread_nums);
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
        float blur_score  = value["blurScore"];
        std::string path = TraverseDir[i];
        
        string _, Name; utils::splitPathAndName(path, _, Name);
	if (blur_score > 50.f) {
	    utils::CopyFile(path, utils::JoinPaths(output_dir,"sharp/"+Name));
	}else if (blur_score <= 50.f && blur_score > 40.f){
	    utils::CopyFile(path, utils::JoinPaths(output_dir, "norm/"+Name));
        }else if (blur_score <= 40.f){
            utils::CopyFile(path, utils::JoinPaths(output_dir, "blur/"+Name));
        }
    }
    DestroyModuleJson(&JsonInfo);
    BlurDestroy(handle);
    return 0;
}







