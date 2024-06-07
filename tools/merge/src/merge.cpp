#include "LIN_Merge.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include "json.h"
#include "utils.h"

using json = nlohmann::json;
using namespace std;

int main(int argc, char **argv) {
    const char *json_dir = argv[1];
    const char *save_dir = argv[2]; 

    int width = 1920; //1920;2000
    int height= 1080; //1080;1500

    tiorb_img_merge_info merge_info;
    int ret = MergeFromPath(width, height, json_dir, save_dir, &merge_info);
    if (ret != 0)
    {
        std::cerr<<"====> MergeFromPath:"<<ret<<std::endl;
        exit(0);
    }
    
    //if (true) goto exit;

    //vector<string> TraverseVec = utils::TraverseDirectory(json_dir);
    //json json_out;
    //for (auto value : TraverseVec){
        //json_out.push_back(value);
    //}
    //int ret = MergeFromJson(width,height,json_out.dump().c_str(),save_dir,&merge_info, 0.5f);
//exit:
    std::cout<<"====>content1<===="<<std::endl;
    std::cout<<merge_info.content<<std::endl;
    std::cout<<"====>content2<===="<<std::endl;
    return ret;
}
