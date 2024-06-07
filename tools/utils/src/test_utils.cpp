#include <iostream>
#include "LIN_utils.h"
#include "utils.h"
#include <string.h>
#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char **argv){
    const char* lut_dir = argv[1];
    const char* img_dir = argv[2];
    const char* out_dir = argv[3];
    
    auto pathVec = utils::TraverseDirectory(img_dir);
    for (auto path : pathVec){
        std::cout<<"============> path:"<<path<<std::endl;
    
        string directory, fileName, extension;
        utils::splitPathAndName(path, directory, fileName);
        string name = string(out_dir)+"/"+fileName;
        applyLutEffect(lut_dir, path.c_str(), name.c_str());
    }
    return 0;
}

