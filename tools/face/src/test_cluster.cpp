#include "linux_api.h"
#include "smart_album_face_cluster.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <vector>
#include <dirent.h>
#include <chrono> 
#include <map>
#include "utils.h"
#include <algorithm>
#include <unistd.h>
#include <stdlib.h>

int bigface = 0;
int facesize = 640;
int model_type = 0;
int forward_type=0;
int ISdetect = 0;
int face_info=0;
int cover_info=1;
const char* model_dir = nullptr;
const char* input_dir = nullptr;
const char* output_dir= "data/results";

std::ofstream ofs("./data/debug/faceTime.csv", std::ios::out);
std::ofstream txt("./data/debug/faceFeature.txt", std::ios::out);

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

void func(std::vector<int>& labelvec, int& num_cluster, tiorb_face_cluster_info& cluster_info, int PearPREFace, int cout) {
    int PREFaceNums = 0;
    bool FLAG = true;
    struct dirent *subptr;
    std::string image_file=std::string(input_dir);
    if (cout!=0) image_file+="/-"+std::to_string(cout);  
    if (access(image_file.c_str(), F_OK)) return;
    DIR *imgDir = opendir(image_file.c_str());
    std::vector<std::vector<float>> feat_vector;
    std::vector<std::string> file_vector;

    struct Handle* handle;
    if (ISdetect){
        handle = GetExtractHandle();
        int init_ret = 0;
	if (bigface){
	    std::cout<<"############### centerface ###############"<<std::endl;
            init_ret = ExtractFaceInit(handle, model_dir, forward_type, model_type, facesize);
	}else{
	    std::cout<<"############### scrfd ###############"<<std::endl;
	    init_ret = ExtractInit(handle, model_dir, forward_type, model_type);
	}

        if (init_ret != 0) printf("######init_ret: %d \n", init_ret);
        std::cout<<"************************* 检测人脸模式 *************************"<<std::endl;
    }else{
	handle = GetFeatureHandle();
        int init_ret = FeatureInit(handle, model_dir, forward_type, model_type);
	if (init_ret != 0) return;
        std::cout<<"************************* 提取特征模式 *************************"<<std::endl;
    }
    while (FLAG)
    {
	if ((subptr = readdir(imgDir)) == 0){
            FLAG = false;
            PearPREFace = PREFaceNums;
        }
        int num_faces = 0;
        if (FLAG){
            std::string image_name = subptr->d_name;
            if (image_name=="."||image_name==".."||image_name=="-1"||image_name=="-2") continue;
	    std::cout << "image_name: " << image_name << std::endl;
            std::string fullname = image_file+"/"+image_name; 
            std::tuple<unsigned char*, int> result = utils::readBuffer(fullname.c_str());
            
            unsigned char* inputimage = std::get<0>(result);
            int size = std::get<1>(result);
            if (ISdetect){
                tiorb_image_feats_info feats_info;
		const auto &timeProcess0 = std::chrono::steady_clock::now();
                int ret = ExtractImageFeats(handle, inputimage, size, &feats_info, 1.5f, false);
		if (ret!=0) std::cout << "ExtractImageFeats: " << ret << std::endl;

		const auto &timeProcess1 = std::chrono::steady_clock::now();
    	        const auto timeProcess2 = (timeProcess1 - timeProcess0).count() / 1000000.0;  
                num_faces = feats_info.num_face;
		if (cover_info && ret==0 && num_faces>0 && cout==0){
		    std::string debug_dir = "data/debug/face-cover/";
		    mkdir(debug_dir.c_str(), 0755);
		    int img_size = feats_info.img_size[0]; // only save first face cover
		    unsigned char img[img_size];
		    for (int j=0;j<img_size;j++) img[j]=feats_info.img[j];
		    try {
		        utils::saveImage(img, img_size, (debug_dir+image_name+".jpg").c_str());
		    }catch(std::exception& e){}
		}
		delete[] inputimage;
                if (ret != 0) continue;
		if (cout==0) ofs<<image_name<<","<<timeProcess2*0.08f<<","<<timeProcess2*0.05f<<","<<timeProcess2*0.15f<<","<<timeProcess2*0.72f<<","<<timeProcess2*1.f<<"\n";

                float* FaceFeature = feats_info.FaceFeature;
                for (int i=0; i<num_faces; i++){
                    file_vector.push_back(fullname);
                    std::vector<float> featvector;
                    for (int j = 0; j < 512; j++) {
                        featvector.push_back(*(FaceFeature + i*512 + j));
                        txt<<*(FaceFeature + i*512 + j)<<" ";
                    }
                    txt<<image_name<<"\n";
                    feat_vector.push_back(featvector);
                }
                ExtractDestroyStruct(&feats_info);
            }else{
                tiorb_face_feature_info feature_info;
                const auto &timeProcess0 = std::chrono::steady_clock::now();
                int ret = ExtractFeature(handle, inputimage, size, &feature_info);
                const auto &timeProcess1 = std::chrono::steady_clock::now();
    	        const auto timeProcess2 = (timeProcess1 - timeProcess0).count() / 1000000.0;
                delete[] inputimage;
		if (ret != 0) continue;
                if (cout== 0) ofs<<image_name<<","<<timeProcess2*0.08f<<","<<timeProcess2*0.05f<<","<<timeProcess2*0.15f<<","<<timeProcess2*0.72f<<","<<timeProcess2*1.f<<"\n";
		file_vector.push_back(fullname);
                std::vector<float> featvector;
                num_faces = 1;
                float* FaceFeature = feature_info.FaceFeature;
                for (int j = 0; j < 512; j++) {
                    featvector.push_back(*(FaceFeature + j));
		    txt<<*(FaceFeature + j)<<" ";
                }
		txt<<image_name<<"\n";
                feat_vector.push_back(featvector);
                FeatureDestroyStruct(&feature_info);
            }
	}
        
        PREFaceNums += num_faces;
        if (PREFaceNums >= PearPREFace){
            float* clusterfeats = new float[PREFaceNums * 512];
            for (int i = 0; i < PREFaceNums; ++i) {
                for (int j = 0; j < 512; ++j) {
                    clusterfeats[512 * i+j] = feat_vector[i][j];
                }
            } 
            std::vector<std::vector<float>>().swap(feat_vector); 
            
            if (num_cluster == 0){
                FaceCluster(PREFaceNums, clusterfeats, &cluster_info, 0.53f);
		delete[] clusterfeats;
                num_cluster = cluster_info.num_cluster;
                for (int i = 0; i < PREFaceNums; i++) {
                    int index = *(cluster_info.labels + i);
                    labelvec.push_back(index);
                    std::string file_path = file_vector[i];
                    std::string::size_type iPos = file_path.find_last_of("/") + 1;
                    std::string filename = file_path.substr(iPos, file_path.length() - iPos);
    	            if (index==-1){
                        std::string destdir = std::string(input_dir)+"/-"+std::to_string(cout+1);
                        mkdir(destdir.c_str(), 0755);
                        copyToDir(file_vector[i].c_str(), (destdir+"/"+filename).c_str());
                    }else{
			std::string image_dir = std::string(output_dir)+"/" + std::to_string(index);
                        mkdir(image_dir.c_str(), 0755);
                        copyToDir(file_vector[i].c_str(), (image_dir +"/"+ filename).c_str());
                    } 
                }
                std::vector<std::string>().swap(file_vector);
            }else{
		float* averagefeats = new float[num_cluster*512];
                float* centerfeats = new float[num_cluster*512];
                int* centerlabels = new int[num_cluster];
                int* labelnums = new int[num_cluster];
                // 获取中间向量
                for (int i = 0; i < num_cluster; ++i) {
                    for (int j = 0; j < 512; ++j) {
                        centerfeats[512 * i + j] = *(cluster_info.CenterFeature + 512 * i + j);
			averagefeats[512 * i + j]= *(cluster_info.AverageFeature + 512 * i + j);						
                    }
                }
                // 获取中间向量每个label包含的数量
	        typedef std::map<int, int> mapIntInt;
	        mapIntInt mapCount;
	        for (int i = 0; i < labelvec.size(); ++i) {
		    mapIntInt::iterator iterator(mapCount.find(labelvec[i]));
		    if (iterator != mapCount.end()){
		       iterator->second++;
		    }else {
                       mapCount[labelvec[i]] = 1;
                    }
	        }
	        int index = 0;
	        for (auto const& element : mapCount){
		    int typeName = element.first;
		    if (typeName != -1){
		        centerlabels[index] = typeName;
		        labelnums[index] = element.second;
		        index++;
		    }
	        }

		int preface_ret = PREFaceCluster(num_cluster, centerfeats, averagefeats, centerlabels, labelnums, PREFaceNums, clusterfeats, &cluster_info, 0.53f);
                
		delete[] clusterfeats;
		delete[] averagefeats;
		delete[] centerlabels;
                delete[] labelnums;
		if (preface_ret != 0) continue;
                int num_all = num_cluster+PREFaceNums;
                for (int i = 0; i < num_all; i++) {
                    if (i<num_cluster) continue;
                    int index = *(cluster_info.labels + i);
                    labelvec.push_back(index);
                    std::string file_path = file_vector[i-num_cluster];
                    std::string::size_type iPos = file_path.find_last_of("/") + 1;
                    std::string filename = file_path.substr(iPos, file_path.length() - iPos);
	            if (index == -1){
                        std::string destdir = std::string(input_dir)+"/-"+std::to_string(cout+1);
                        mkdir(destdir.c_str(), 0755);
                        copyToDir(file_vector[i-num_cluster].c_str(), (destdir+"/"+filename).c_str());
                    }else{
			std::string image_dir = std::string(output_dir)+"/"+std::to_string(index);
                        mkdir(image_dir.c_str(), 0755);
                        copyToDir(file_vector[i-num_cluster].c_str(), (image_dir+"/"+filename).c_str());
                    }
                }
                num_cluster = cluster_info.num_cluster;
		std::vector<std::string>().swap(file_vector);
            }
            PREFaceNums = 0;
        }
    }
    closedir(imgDir);
    if (ISdetect){ 
        ExtractDestroyModel(handle);  
    }else{
        FeatureDestroyModel(handle);
    }
}

int main(int argc, char **argv){
    int num_cluster=0;
    int PearPREFace=atoi(argv[1]);
    int sum = atoi(argv[2]);
    model_dir = argv[3];
    input_dir = argv[4];
    ISdetect = atoi(argv[5]);
    forward_type=atoi(argv[6]);
    bigface=atoi(argv[7]);
    facesize=atoi(argv[8]);
    std::vector<int> labelvec;

    // LNAS03BAD2D84E9DA21972A7D4D21C287B0D6BA913E3
    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2", "");
    std::cout<<"鉴权##: "<< ret<<std::endl;
    ofs << "image_name," << "detect(ms),"<< "landmark(ms),"<< "quality(ms),"<< "feature(ms),"<< "all(ms)" << "\n";
    
    tiorb_face_cluster_info cluster_info;
    for (int i=0; i<sum; i++){
	if (i == sum-1) PearPREFace = 1000;
        func(labelvec,num_cluster,cluster_info,PearPREFace,i);
    }
    ofs.close();
    txt.close();
    ClusterDestroy(&cluster_info);
    return 0;
}







