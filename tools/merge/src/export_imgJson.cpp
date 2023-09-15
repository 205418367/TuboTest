#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include "LIN_Merge.h"
#include "linux_api.h"
#include "utils.h"
#include "json.h"

int main(int argc, char **argv){
    const char *model_dir = argv[1]; //"models/";
    const char *input_dir = argv[2]; //"data/cluster";
    const char *json_dir  = argv[3]; //"data/results";
   
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    if (ret != 0) {
        printf("InitParams failed %d \n", ret);
        exit(1);
    }
    Handle* handle = GetHandle();
    ret = FaceFastInit(handle, model_dir, 0);
    if (ret != 0) {
        printf("Face Init failed %d \n", ret);
        exit(1);
    }

    struct dirent *dir_entry;
    DIR *dir = opendir(input_dir);
    if (!dir) {
        printf("unable to open directory\n");
        exit(1);
    }
    
    nlohmann::json info_json;
    std::ofstream write(std::string(json_dir)+"/faceinfo.json");
    while ((dir_entry = readdir(dir)) != 0) {
        if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")) {
	    char path[1000];
            path[0] = '\0';
            strcat(path, input_dir);
            strcat(path, "/");
            strcat(path, dir_entry->d_name);
	    
	    std::tuple<unsigned char*, int> result = utils::readBuffer(path);
            unsigned char* img = std::get<0>(result);
            int size = std::get<1>(result);
	    tiorb_FaceCheck_info face_info;
            ret = FaceCheckOut(handle, img, size, &face_info, 0);
	    if (ret != 0 || face_info.num_faces == 0) {
                printf("FaceCheckOut no face or failed %s \n", dir_entry->d_name);
		continue;
            }
	    int x = face_info.location[0];
	    int y = face_info.location[1];
	    int w = face_info.location[2];
	    int h = face_info.location[3];          

	    nlohmann::json infojson;
	    infojson["path"] = path;
	    infojson["rect"].push_back(x);
	    infojson["rect"].push_back(y);
	    infojson["rect"].push_back(w);
	    infojson["rect"].push_back(h);
	    info_json["photos"].push_back(infojson);
	    FaceDestructStruct(&face_info);
	}
    }
    write << info_json.dump(); write.close();
    FaceDestructModel(handle);
    return 0;
}

