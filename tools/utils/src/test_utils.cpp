#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include "LIN_utils.h"
#include "utils.h"
#include <string.h>

int main(int argc, char **argv){
    int target = atoi(argv[1]);
    const char *input_dir = argv[2];
    const char *output_dir= argv[3];
   
    TIME startTime = utils::GetCurrentTime();
    std::tuple<unsigned char*, int> result = utils::readBuffer(input_dir);
    unsigned char* ptr = std::get<0>(result); 
    int value = std::get<1>(result);

    int ret = ResizeImageBuffer(ptr, value, target, output_dir);
    
    if (ret != 0) std::cout<<"************* error! ************* "<< ret <<std::endl;
    int64_t Duration = utils::GetDurationTime(startTime); 
    std::cout<<" Duration:"<<Duration<<std::endl;
    return 0;
}

