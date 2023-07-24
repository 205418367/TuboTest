#include "linux_api.h"
#include "smart_album_img_cls.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <vector>
#include <dirent.h>
#include <chrono> 
#include "utils.h"
#include <unistd.h>
#include <stdlib.h>


std::vector<std::string> labels{
      "瀑布",  "雪景",  "草原",  "水下",  "建筑",  "日出日落",  "天空",  "绿植",  "鲜花",  "夜景",  "自拍",
      "舞台", "烟花", "烛光", "家居室内", "文本", "二维码", "屏幕", "健身房", "篝火", "多人", "桥",
      "蛋糕", "露营", "交通车辆", "卡通", "沙漠", "饮料", "喷泉", "水果", "游乐场", "儿童", "操场",
      "球类运动", "街道", "地铁", "超市", "游泳", "蔬菜", "婚礼", "船", "其他动物", "狗", "江河",
      "聚餐", "盆栽", "截图", "猫", "微距", "美食", "沙滩", "山峰", "单人"};

void copyToDir(const char* srcdir, const char* dstdir){
    FILE *src, *dst;
    src = fopen(srcdir, "rb");
    dst = fopen(dstdir, "wb");
    unsigned char c;
    int i;
    while(1){
	i = fread(&c, 1, 1, src);
	if (i <= 0) break;
	fwrite(&c, 1, 1, dst);
    }
    fclose(src);
    fclose(dst);
}

int main(int argc, char **argv) {
    int input_type =  atoi(argv[1]); // 0 输入文件夹; 1 输入文件; 2 输入视频buffer; 3 输入视频路径;
    const char* root_path = argv[2];
    const char* input_dir = argv[3];
    const char* image_res = argv[4];
    const char* input_img = argv[3];
    const char* input_vedio=argv[3];
    int forward_type=atoi(argv[5]);

    int ret = InitParams(root_path, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    printf("####InitParams_res %d \n", ret); 
    struct Handle* handle = GetSceneHandle();
    ret = SceneInit(handle, root_path, forward_type);
    if (ret != 0) std::cout<<"####SceneInit "<<ret<<std::endl; 

    std::string TxtPath = "./data/debug/sceneTime.csv";
    std::ofstream ofs(TxtPath, std::ios::out);
    ofs << "image_name,"<< "speed(ms)" << "\n";
    
    if (input_type == 0) 
    {
	struct dirent *subptr;
	if (access(input_dir, F_OK)) printf("#### file is not exist");
        DIR *imgDir = opendir(input_dir);
        while ((subptr = readdir(imgDir)) != 0)
        {
            std::string image_name = std::string(subptr->d_name);
            if(image_name=="."||image_name==".."||image_name=="-1"||image_name=="-2") continue;
            std::string fullname = std::string(input_dir)+"/"+image_name;
            printf("image_name: %s \n", image_name.c_str());
            std::tuple<unsigned char*, int> result = utils::readBuffer(fullname.c_str());
            unsigned char* inputimage = std::get<0>(result);
            int size = std::get<1>(result);

            tiorb_scene_detect_info detect_info;
	    const auto &timeProcess0 = std::chrono::steady_clock::now(); 
            ret = SceneDetect(handle, inputimage, size, &detect_info);
            if (ret != 0) std::cout<<"####SceneDetect "<<ret<<std::endl; 
	    const auto &timeProcess1 = std::chrono::steady_clock::now();
            delete[] inputimage;
        
            // 获取识别结果的数量
            int num_scenes = detect_info.num_scenes;
            for (int i = 0; i < num_scenes; i++) 
	    {
                // 获取识别结果的label
                int label = *(detect_info.index + i); 
                std::string image_dir = std::string(image_res) +"/"+ labels[label];
                mkdir(image_dir.c_str(), 0755);
                copyToDir(fullname.c_str(), (image_dir+"/"+image_name).c_str());
		ofs <<image_name<<","<< (timeProcess1 - timeProcess0).count() / 1000000.0 << "\n";
            }
	    if (num_scenes==0) 
	    {
		mkdir("data/results/-1", 0755);
		copyToDir(fullname.c_str(), ("data/results/-1/"+image_name).c_str());
	    }
            SceneDestroyStruct(&detect_info); 
        }
    }

    tiorb_scene_detect_info detect_info;
    if (input_type == 1) {
        std::tuple<unsigned char*, int> result = utils::readBuffer(input_img);
        const unsigned char* inputimage = (const unsigned char*)std::get<0>(result);
        int size = std::get<1>(result);
        ret = SceneDetect(handle, inputimage, size, &detect_info);
        delete[] inputimage;
    }
    else if (input_type == 2) {
        std::tuple<unsigned char*, int> result = utils::readBuffer(input_vedio);
        const unsigned char* inputimage = (const unsigned char*)std::get<0>(result);
        int size = std::get<1>(result);
        ret = VideoBufferDetect(handle, inputimage, size, 0.4f, &detect_info);
        std::cout << "######## VideoBufferDetect ######## " << ret << std::endl;
        delete[] inputimage;
    }
    else if (input_type == 3) {
        ret = VideoPathDetect(handle, input_vedio, 0.4f, &detect_info);
        std::cout << "######## VideoPathDetect ######## " << ret << std::endl;
    }

    if (input_type != 0) {
        int num_scenes = detect_info.num_scenes;
        std::cout << "num_scenes: " << num_scenes << std::endl;
        for (int i = 0; i < num_scenes; i++) {
            int label = *(detect_info.index + i);
            if (label > 0) {
                std::string name = labels[label];
                std::cout << "label: " << name << std::endl;
                if (input_type != 1) {
                    std::cout << "accuracy: " << *(detect_info.accuracy + i) << std::endl;
                }
             }
        }
        SceneDestroyStruct(&detect_info);
    }
    ofs.close();
    // 释放占用资源
    SceneDestroyModel(handle);
    std::cout << "#### success! ####" << std::endl;
    return 0;
}
