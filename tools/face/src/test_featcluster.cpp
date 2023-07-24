#include <dirent.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "linux_api.h"
#include "smart_album_face_cluster.h"

using namespace std;

void split(string str,string pattern,vector<float>& result1,vector<string>& result2)
{
    std::string::size_type pos;
    str += pattern;
    int size = str.size();
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
	    sum += 1;
            std::string s = str.substr(i, pos - i);
            if (sum < 513) {
		result1.push_back(atof(s.c_str()));
	    }else{
		result2.push_back(s);
	    }
            i = pos + pattern.size() - 1;
        }
    }
}

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


void func(const char* data_dir1, const char* data_dir2, const char* feature_txt1, const char* feature_txt2) {
    std::string content;
    std::vector<float>  result1;
    std::vector<std::string> result2;
    std::ifstream txt1(feature_txt1);
    if (txt1){
        while (getline(txt1, content, '\n')){
            //if (content == "") continue;
	    split(content, " ", result1, result2);
        }
    }else{
        std::cout << "Open file1 faild." << std::endl;
	return;
    }
    txt1.close();
    int FaceNum1 = result2.size();

    std::ifstream txt2(feature_txt2);
    if (txt2){
        while (getline(txt2, content, '\n')){
	    //if (content == "") continue;
	    split(content, " ", result1, result2);
        }
    }else{
        std::cout << "Open file2 faild." << std::endl;
	return;
    }
    txt2.close();

    int FaceNums = result2.size();
    float* clusterfeats = new float[FaceNums*512];
    for (int i = 0; i < FaceNums; ++i) {
	for (int j = 0; j < 512; ++j) {
	    clusterfeats[512*i+j] = result1[i*512+j];
	}
    } 
    tiorb_face_cluster_info cluster_info;
    FaceCluster(FaceNums, clusterfeats, &cluster_info, 0.53f);
    delete clusterfeats;

    for (int i = 0; i < FaceNums; i++) {
	int index = *(cluster_info.labels + i);
	std::string filename = result2[i];
	if (index==-1){
	    std::string destdir = std::string(data_dir1)+"/-1";
	    mkdir(destdir.c_str(), 0755);
	    if (i<FaceNum1){
		copyToDir((std::string(data_dir1)+"/"+filename).c_str(), (destdir+"/"+filename).c_str());
	    }else{
		copyToDir((std::string(data_dir2)+"/"+filename).c_str(), (destdir+"/"+filename).c_str());
	    }
	}else{
	    std::string image_dir = "data/results/" + std::to_string(index);
	    mkdir(image_dir.c_str(), 0755);
	    if (i<FaceNum1){
		copyToDir((std::string(data_dir1)+"/"+filename).c_str(), (image_dir +"/"+ filename).c_str());
	    }else{
		copyToDir((std::string(data_dir2)+"/"+filename).c_str(), (image_dir +"/"+ filename).c_str());
	    }
	} 
    }
}

int main(int argc, char **argv){
    const char* data_dir1 = argv[1];
    const char* data_dir2 = argv[2];
    const char* feature_txt1=argv[3];
    const char* feature_txt2=argv[4];
    int InitParams_res = InitParams("data/results/", "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    std::cout<<"鉴权##: "<< InitParams_res<<std::endl;
    func(data_dir1, data_dir2, feature_txt1, feature_txt2);
    return 0;
}







