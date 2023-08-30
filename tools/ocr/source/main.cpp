#include "OcrLite.h"
#include "json.h"
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>

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

void drawImage(const char* imgfile, tiorb_img_ocr_info &ocr_info){
    json data = nlohmann::json::parse(ocr_info.content);
    int numrect = data["numrect"];
    std::string outpath = "data/results/"+std::string(imgfile)+".txt";
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

    struct dirent *dir_entry;
    DIR *dir = opendir(input_dir);
    if (!dir) {
        printf("unable to open directory\n");
        exit(1);
    } 

    Handle *handle = GetOcrImgHandle();
    int ret = OcrImgInit(handle, model_dir, 0);
    if (ret != 0) return ret;

    while ((dir_entry = readdir(dir)) != 0) {
        if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")) {
            char path[1000];
            path[0] = '\0';
            strcat(path, input_dir);
            strcat(path, "/");
            strcat(path, dir_entry->d_name);
	    std::cout<< dir_entry->d_name <<std::endl;

	    tiorb_img_ocr_info ocr_info;
            ret = OcrImgInferPath(handle, path, &ocr_info);
            if (ret != 0) return ret;
            drawImage(dir_entry->d_name, ocr_info);
	    OcrImgDestroyStruct(&ocr_info);
        }
    }
    return 0;
}
