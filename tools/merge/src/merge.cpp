#include "LIN_Merge.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include "json.h"

using json = nlohmann::json;

int main(int argc, char **argv) {
    const char *json_dir = argv[1];
    const char *save_dir = argv[2]; 

    int width = 1920;
    int height= 1080;

    tiorb_img_merge_info merge_info;
    int ret = MergeFromPath(width, height, json_dir, save_dir, &merge_info, 0.5f);
    //if (true) goto exit;

    //std::ifstream jsonfile(json_dir);
    //json data = json::parse(jsonfile);
    //jsonfile.close();
    //std::string json_info = data.dump();
    //int ret = MergeFromJson(width,height,json_info.c_str(),save_dir,&merge_info, 0.5f);
//exit:
    std::cout<<"====>content2<===="<<std::endl;
    std::cout<<merge_info.content<<std::endl;
    std::cout<<"====>content1<===="<<std::endl;
    return ret;
}
