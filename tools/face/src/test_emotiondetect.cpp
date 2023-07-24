#include "linux_api.h"
#include "utils.h"
#include <time.h>
#include <vector>
#include <chrono>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>


int main(int argc, char **argv) {
    const char *dir_name = "data/cluster";
    const char *imageres = "data/results";
    char* model_file = argv[1];
    int InitParams_res = InitParams(model_file, "ca3f59c7636cca88", "8e6487c5e3425dd3031221d2ddb43328");
    std::cout<<"########## 鉴权 "<< InitParams_res<<std::endl;

    struct dirent *dir_entry;
    DIR *dir = opendir(dir_name);
    if (!dir) {
        printf("unable to open directory\n");
        exit(1);
    }
    
    Handle* handle = GetEmotionHandle();
    int ret = EmotionInit(handle, model_file, 0);
    if (ret != 0) {
        printf("Face Init failed %d \n", ret);
        exit(1);
    }
    while ((dir_entry = readdir(dir)) != 0) {
        if (strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")) {
	    char path[1000];
            path[0] = '\0';
            strcat(path, dir_name);
            strcat(path, "/");
            strcat(path, dir_entry->d_name);
	    
	    std::tuple<unsigned char*, int> result = utils::readBuffer(path);
    	    unsigned char* inputimage = std::get<0>(result);
            int size = std::get<1>(result);
	    const auto &timeProcess0 = std::chrono::steady_clock::now();
            tiorb_emotion_detect_info emotion_info;
            ret = EmotionDetect(handle, inputimage, size, &emotion_info);
            if (ret != 0) {
		delete inputimage;
		continue;
	    }
	    const auto &timeProcess1 = std::chrono::steady_clock::now();
    	    printf("infer time = %.3lf [msec]\n", (timeProcess1 - timeProcess0).count() / 1000000.0);
	    DrawRectangle(inputimage,size,2,emotion_info.num_faces,emotion_info.label,emotion_info.location,(char*)imageres);
	    delete inputimage;
	    EmotionDestroyStruct(&emotion_info);
	}
    }
    EmotionDestroyModel(handle);
    return 0;
}
