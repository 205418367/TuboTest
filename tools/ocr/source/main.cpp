#include "OcrLite.h"
#include "json.h"
#include "utils.h"
#include <fstream>

using namespace std;
using json = nlohmann::json;

bool writeFile(const std::string& filename, const char* content) {
    std::ofstream outfile(filename, std::ios_base::app);
    if (outfile.is_open()) {
        outfile << content;
        outfile.close();
        return true;
    } else {
        std::cout << "无法打开文件进行写入" << std::endl;
        return false;
    }
}

void drawImage(const string& output, const string& imgfile, tiorb_img_ocr_info &ocr_info){
    json data = nlohmann::json::parse(ocr_info.content);
    int numrect = data["numrect"];
    std::string outpath = utils::JoinPaths(output, imgfile+".txt");
    if (data["numrect"] == 0) {
	std::cout<<"1 =================="<<std::endl;
	writeFile(outpath, "");
	std::cout<<"2 =================="<<std::endl;
    }else{
	for (int i=0; i<numrect; i++){
            std::string text = data["txtrect"]["text"][i];
            writeFile(outpath, (text+"\n").c_str());
	}
    }
}

int main(int argc, char **argv) {
    const char *model_dir = argv[1];
    const char *input_dir = argv[2]; 
    const char *output_dir= argv[3]; 

    std::cout<< "====>begning"<< std::endl;
    Handle *handle = GetOcrHandle();
    int ret = OcrInit(handle, model_dir, 0);
    if (ret != 0) {
        std::cerr << "Ocr Init: "<< ret << std::endl;
        exit(1);
    } 

    std::cout<< "====>input_dir:"<< input_dir << std::endl;
    vector<string> TraverseVec = utils::TraverseDirectory(input_dir);
    std::cout<< "====>nums:" << TraverseVec.size() << std::endl;
    for (auto path : TraverseVec){
	tiorb_img_ocr_info ocr_info;
        ret = OcrInferPath(handle, path.c_str(), &ocr_info);
        std::cout<< "====>" << path << " ret:" << ret << std::endl;
        if (ret != 0) continue;

        string _, Name; utils::splitPathAndName(path, _, Name);
        drawImage(output_dir, Name, ocr_info);
        OcrDestroyStruct(&ocr_info);
    }
    return 0;
}
