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
    const char *output_path= argv[3];
   
    struct dirent *dir_entry;
    DIR *dir = opendir(input_dir);
    if (!dir) {
        printf("unable to open directory\n");
        exit(1);
    } 

    while ((dir_entry = readdir(dir)) != 0) {
        if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")) {
            char path[1000];
            path[0] = '\0';
            strcat(path, input_dir);
            strcat(path, "/");
            strcat(path, dir_entry->d_name);
	    std::cout<< dir_entry->d_name <<std::endl;
	    std::string outpath = std::string(output_path)+"/"+dir_entry->d_name;

	    //int ret = FrameExtraction(path, target, output_path);
	    //int ret = ResizeImagePath(path, target, output_path);
	    int ret = transferHeic2Jpg(path, outpath.c_str(), 100);
            if (ret !=0) std::cout<<"************* error! ************* "<< ret <<std::endl;
	    getchar();
        }
    }
    return 0;
}

