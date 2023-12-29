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
    TiorbModuleJsonInfo JsonInfo;
    int ret = ResizeImageBatch(input_dir, 800, output_dir, &JsonInfo, 4);
    //int ret = FrameExtraction(path.c_str(), 800, output_dir, 10);
    if (ret !=0) std::cout<<"************* error! ************* "<< ret <<std::endl;
    int64_t Duration = utils::GetDurationTime(startTime); 
    std::cout<<"====> thread_nums:"<<4<<" Duration:"<<Duration<<std::endl;
    return 0;
}

