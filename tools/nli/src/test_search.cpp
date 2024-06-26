#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include "LIN_nlicluster.h"
#include "LIN_nliimage.h"
#include "LIN_nlitxt.h"
#include "utils.h"
#include "linux_api.h"
#include "json.h"

using json = nlohmann::json;
using namespace std;

int main(int argc, char **argv){
    int thread_nums = atoi(argv[1]);
    const char *model_dir = argv[2]; 
    const char *input_data= argv[3]; 
    float thresh = atof(argv[4]);
    const char *input_dir= argv[5]; 
    const char *output_dir= argv[6];
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    if (ret != 0) {
        std::cerr << "Failed to Init Params: "<< ret << std::endl;
        exit(1);
    } 
    
    std::string inputStr(input_data);
    if (!utils::FileExist(inputStr)){
        std::cerr << "File Not Exist!" << std::endl;
        exit(1);
    }
    
    int type=(inputStr.substr(inputStr.length()-4)==".txt");
    std::string imgfeat = utils::JoinPaths(output_dir,"imgfeat.json");
    bool FileExist = true;
    try {
        int size = (int)fs::file_size(imgfeat);
        if (size == 0) {
            FileExist = false;
        }
    } catch (std::filesystem::filesystem_error& e) {
        FileExist = false;
    }
    
    if (!FileExist){
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
    
    std::ifstream jsonfile(imgfeat);
    json data = json::parse(jsonfile);
    jsonfile.close();
    std::vector<float> feat_vector;
    std::vector<std::string> file_vector;
    vector<string> TraverseDir = utils::TraverseDirectory(input_dir);
    for (size_t i=0; i<data.size(); i++)
    {
        auto value = data[i];
        if ( value["result"] != 0) continue;
        std::vector<float> feats_vec = value["feature"].get<std::vector<float>>();
        feat_vector.insert(feat_vector.end(), feats_vec.begin(), feats_vec.end());
        file_vector.push_back(TraverseDir[i]);
    }  

    if (type == 1){
	struct Handle *handle = GetNliTxtHandle(); 
        ret = NliTxtInit(handle, model_dir, 0);
	//ret = NliENTxtInit(handle, model_dir, 0);
        if (ret != 0) {
            std::cerr << "Failed to NliZHTxtInit: "<< ret << std::endl;
            exit(1);
        } 

        std::ifstream file(input_data); std::string line;
        while (std::getline(file, line)) {
            tiorb_txt_feat_info feat;
            ret = NliTxtInfer(handle, line.c_str(), &feat);
            if (ret != 0) {
                std::cerr << "Failed to NliTxtInfer: "<< ret << std::endl;
                exit(1);
            } 
            tiorb_nli_search_info search_info;
	    SearchImg(feat.txtFeature, feat_vector.size()/1024, feat_vector.data(), &search_info, thresh, 300);
	    int numshow = search_info.numshow;
            int* indexs = search_info.indexs;
            float* scores=search_info.scores;
            for (int i=0; i<numshow; i++){
                std::string imgpath = file_vector[indexs[i]];
                std::cout<<"====>name:"<<imgpath<<" score:"<<scores[i]<<std::endl;
                std::string dir, name; utils::splitPathAndName(imgpath, dir, name);
                utils::CreateDirectory(utils::JoinPaths(output_dir,line));
                std::string target = utils::JoinPaths(output_dir, line+"/"+std::to_string(i)+"_"+name);
                utils::CopyFile(imgpath, target);
            }
            SearchDestroy(&search_info);
        }
        file.close();
    }else{
	struct Handle *handle = GetNliImgHandle(); 
        ret = NliImgInit(handle, model_dir, 0);
	//ret = NliENImgInit(handle, model_dir, 0);
        if (ret != 0) {
            std::cerr << "Failed to NliENImgInit: "<< ret << std::endl;
            exit(1);
        }
	tiorb_img_feat_info feat;
        ret = NliImgInferPath(handle, input_data, &feat);
        if (ret != 0) {
            std::cerr << "Failed to NliImgInfer: "<< ret << std::endl;
            exit(1);
        }
        tiorb_nli_search_info search_info;
	SearchImg(feat.imgFeature, feat_vector.size()/1024, feat_vector.data(), &search_info, thresh, 30);
	NliImgDestroyStruct(&feat);
	NliImgDestroy(handle);
	
	int numshow = search_info.numshow;
        int* indexs = search_info.indexs;
        float* scores=search_info.scores;
        for (int i=0; i<numshow; i++){
            std::string imgpath = file_vector[indexs[i]];
            std::cout<<"====>name:"<<imgpath<<" score:"<<scores[i]<<std::endl;
            std::string dir, name; utils::splitPathAndName(imgpath, dir, name);
            std::string target = utils::JoinPaths(output_dir, std::to_string(i)+"_"+name);
            utils::CopyFile(imgpath, target);
        }
        SearchDestroy(&search_info);
    }
    return 0;
}

