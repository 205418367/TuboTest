#include <iostream>
#include <vector>
#include <sstream>
#include "LIN_nlitxt.h"
#include "json.h"
#include "linux_api.h"
#include "utils.h"

void readTxt(const char* txtfile, std::vector<std::string>& names, std::vector<int>& label){
    std::string content, tmp;
    std::ifstream txt(txtfile);
    if (txt){
        while (getline(txt, content, '\n')){
	    std::stringstream ss(content);
            ss>>tmp;
	    names.push_back(tmp);
	    ss>>tmp;
	    label.push_back(atoi(tmp.c_str()));
        }
    }else{
        std::cout << "Open file faild." << std::endl;
	return;
    }
    txt.close();
}

int main(int argc, char **argv){
    const char *model_dir = argv[1]; 
    const char *txtfile   = argv[2];
    const char *image_ret = argv[3];
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    printf("####InitParams: %d \n", ret); 

    struct Handle *handle = GetNliTxtHandle(); 
    ret = NliTxtInit(handle, model_dir, 0);
    if (ret != 0) printf("************* NliTxtInit failed! ************* \n");

    int numtxts = 0;
    nlohmann::json info_json, tmp_json;
    info_json["txt"]=tmp_json;
    std::ofstream write(std::string(image_ret)+"/albumfeat.json");

    std::vector<std::string> names;
    std::vector<int> labels;
    readTxt(txtfile, names, labels);
    for (auto keyword : names){
        tiorb_txt_feat_info feat;
        ret = NliTxtInfer(handle, keyword.c_str(), &feat);
        if (ret != 0) printf("************* NliTxtFeature failed! ************* \n");

        std::cout<< keyword << " " << labels[numtxts] <<std::endl;
        nlohmann::json infojson;
        infojson["label"] = labels[numtxts];
        infojson["feats"] = std::vector<float>(feat.txtFeature,feat.txtFeature+1024);
        info_json["txt"].push_back(infojson);
	numtxts += 1;
	NliTxtDestroyStruct(&feat);
    }
    info_json["numtxts"] = numtxts;
    write << info_json.dump();
    write.close();
    NliTxtDestroy(handle);
    return 0;
}

