#include <vector>
#include <map>
#include <algorithm>
#include "utils.h"
#include "linux_api.h"
#include "smart_album_face_cluster.h"
#include <string>
#include <string.h>

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
//std::ofstream txt("./data/debug/faceFeature.txt", std::ios::out);

void func(std::vector<int>& labelvec, int& num_cluster, tiorb_face_cluster_info& cluster_info, int PearPREFace, int cout) {
    int PREFaceNums = 0;
    bool FLAG = true;

    std::string image_file=std::string(input_dir); 
    if (cout!=0)image_file=utils::JoinPaths(input_dir,std::to_string(cout));
    if (!utils::DirectoryExist(image_file)) return;

    vector<string> TraverseVec = utils::TraverseDirectory(image_file);
    std::vector<std::vector<float>> feat_vector;
    std::vector<std::string> file_vector;
    std::vector<int> rect_vector;

    int init_ret=0; struct Handle* handle;
    if (ISdetect){
        handle = GetExtractHandle();
	init_ret = ExtractInit(handle, model_dir, forward_type, model_type);
        std::cout<<"************************* 检测人脸模式 *************************"<<std::endl;
    }else{
	handle = GetFeatureHandle();
        init_ret = FeatureInit(handle, model_dir, forward_type, model_type);
        std::cout<<"************************* 提取特征模式 *************************"<<std::endl;
    }
    if (init_ret != 0) {printf("######init_ret: %d \n", init_ret); return;}

    while (FLAG)
    {
	if (TraverseVec.size() == 0){
            FLAG = false;
            PearPREFace = PREFaceNums;
        }
        int num_faces = 0;
        if (FLAG){
            std::string fullname = TraverseVec[0]; TraverseVec.erase(TraverseVec.begin());
	    std::cout << "fullname: " << fullname << std::endl;
            std::tuple<unsigned char*, int> result = utils::readBuffer(fullname.c_str()); 
            unsigned char* inputimage = std::get<0>(result);
            int size = std::get<1>(result);

            if (ISdetect){
                tiorb_image_feats_info feats_info;
		TIME startTime = utils::GetCurrentTime();
                int ret = ExtractImageFeats(handle, inputimage, size, &feats_info, 1.5f, false);
		int64_t endTime = utils::GetDurationTime(startTime);
                delete[] inputimage; if (ret != 0) continue;
		if (cout==0) ofs<<fullname<<","<<endTime*0.08f<<","<<endTime*0.05f<<","<<endTime*0.15f<<","<<endTime*0.72f<<","<<endTime<<"\n";

                num_faces = feats_info.num_face;
                float* FaceFeature = feats_info.FaceFeature;
                int* location = feats_info.location;
                for (int i=0; i<num_faces; i++){
                    file_vector.push_back(fullname);
		    for (int n=0; n<14; n++) rect_vector.push_back(location[i*14+n]); 
                    std::vector<float> featvector;
                    for (int j = 0; j < 512; j++) {
                        featvector.push_back(*(FaceFeature + i*512 + j));
                        //txt<<*(FaceFeature + i*512 + j)<<" ";
                    }
                    //txt<<fullname<<"\n";
                    feat_vector.push_back(featvector);
                }
                ExtractDestroyStruct(&feats_info);
            }else{
                tiorb_face_feature_info feature_info;
                TIME startTime = utils::GetCurrentTime();
                int ret = ExtractFeature(handle, inputimage, size, &feature_info);
                int64_t endTime = utils::GetDurationTime(startTime);
		delete[] inputimage; if (ret != 0) continue;
                if (cout==0) ofs<<fullname<<","<<endTime*0.08f<<","<<endTime*0.05f<<","<<endTime*0.15f<<","<<endTime*0.72f<<","<<endTime<<"\n";
                num_faces = 1;
		file_vector.push_back(fullname);
                std::vector<float> featvector;
                float* FaceFeature = feature_info.FaceFeature;
                for (int j = 0; j < 512; j++) {
                    featvector.push_back(*(FaceFeature + j));
		//    txt<<*(FaceFeature + j)<<" ";
                }
		//txt<<fullname<<"\n";
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
                    int label = *(cluster_info.labels + i);
                    labelvec.push_back(label);
                    string directory, filename;
		    utils::splitPathAndName(file_vector[i], directory, filename);
    	            if (label==-1){
                        std::string destdir = utils::JoinPaths(input_dir,std::to_string(cout+1));
			 utils::CreateDirectory(destdir);
                        utils::CopyFile(file_vector[i], utils::JoinPaths(destdir,filename));
                    }else{
                        std::string imagedir = utils::JoinPaths(output_dir,std::to_string(label));
			 utils::CreateDirectory(imagedir);
                        //utils::CopyFile(file_vector[i], utils::JoinPaths(imagedir,filename));
                        int rect[4]; for (int j=i*14;j<i*14+4;j++) rect[j-i*14]=rect_vector[j]; 
                        DrawRectangle(file_vector[i].c_str(), rect, utils::JoinPaths(imagedir,filename+".jpg").c_str());
                        
                        //Handle* handle = GetPhotoHandle();
                        //int FaceRect[14]; for (int j=i*14;j<i*14+14;j++) FaceRect[j-i*14]=rect_vector[j]; 
                        //tiorb_face_photo_info photo_info;
                        //PhotoExtractPath(handle, file_vector[i].c_str(), FaceRect, &photo_info);
                        //PhotoDestroyStruct(&photo_info);
                    } 
                }
                std::vector<std::string>().swap(file_vector);
		std::vector<int>().swap(rect_vector);
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
                    int label = *(cluster_info.labels + i);
                    labelvec.push_back(label);
                    int n=i-num_cluster; std::string filepath = file_vector[i-num_cluster];
                    string directory, filename;
		    utils::splitPathAndName(filepath, directory, filename);
	            if (label == -1){
                        std::string destdir = utils::JoinPaths(input_dir,std::to_string(cout+1));
                        utils::CreateDirectory(destdir);
                        utils::CopyFile(file_vector[i-num_cluster], utils::JoinPaths(destdir,filename));
                    }else{
                        std::string imagedir = utils::JoinPaths(output_dir,std::to_string(label));
                        utils::CreateDirectory(imagedir);
                        //utils::CopyFile(file_vector[i-num_cluster], utils::JoinPaths(imagedir,filename));
                        int rect[4]; for (int j=n*4;j<n*4+4;j++) rect[j-n*4]=rect_vector[j]; 
                        DrawRectangle(file_vector[i-num_cluster].c_str(), rect, utils::JoinPaths(imagedir,filename+".jpg").c_str());
                    }
                }
                num_cluster = cluster_info.num_cluster;
		std::vector<std::string>().swap(file_vector);
		std::vector<int>().swap(rect_vector);
            }
            PREFaceNums = 0;
        }
    }
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
    facesize=atoi(argv[7]);
    std::vector<int> labelvec;

    int ret = InitParams(model_dir, "d2f73afa5d59cab5", "4523eac20d03df81c225d9260284f759");
    if (ret != 0) {
        std::cerr << "Failed to Init Params: "<< ret << std::endl;
        exit(1);
    } 
    ofs << "fullname, " << "detect(ms), "<< "landmark(ms), "<< "quality(ms), "<< "feature(ms), "<< "all(ms)" << "\n";
    
    tiorb_face_cluster_info cluster_info;
    for (int i=0; i<sum; i++)
    {
	if (i == sum-1) PearPREFace = 10000;
        func(labelvec,num_cluster,cluster_info,PearPREFace,i);
    }
    ofs.close();
    ClusterDestroy(&cluster_info);
    return 0;
}







