#include "linux_api.h"
#include "LIN_GetFeature.h"
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
    int InitParams_res = InitParams(model_file, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    std::cout<<"########## 鉴权 "<< InitParams_res<<std::endl;

    struct dirent *dir_entry;
    DIR *dir = opendir(dir_name);
    if (!dir) {
        printf("unable to open directory\n");
        exit(1);
    }
    
    Handle* handle = GetHandle();
    Handle* photo = GetPhotoHandle();
    int ret = FaceFastInit(handle, model_file, 0);
    ret = PhotoInit(photo, model_file, 0);
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
            tiorb_FaceCheck_info face_info;
            ret = FaceCheckOut(handle, inputimage, size, &face_info, false);
	    tiorb_face_photo_info photo_info;
            
            for (int i=0; i<face_info.num_faces; i++){
		int* location = new int[4];
		for (int j=0; j<4; j++){
		    location[j] = face_info.location[i*14+j];
		}
		ret = PhotoExtract(photo, inputimage, size, location, &photo_info, 1.5f, 112);
	    }
            if (ret != 0) {
		delete inputimage;
		continue;
	    }
	    const auto &timeProcess1 = std::chrono::steady_clock::now();
    	    printf("infer time = %.3lf [msec]\n", (timeProcess1 - timeProcess0).count() / 1000000.0);
	    DrawRectangle(inputimage,size,0,face_info.num_faces,nullptr,face_info.headlocate,(char*)imageres);
	    delete inputimage;
	    FaceDestructStruct(&face_info);
	    PhotoDestroyStruct(&photo_info);
	}
    }
    FaceDestructModel(handle);
    PhotoDestroyModel(photo);
    return 0;
}
