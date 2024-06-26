#include <vector>
#include <map>
#include <algorithm>
#include "utils.h"
#include "linux_api.h"
#include "smart_album_face_cluster.h"
#include <string>
#include <string.h>

int model_type = 0;
int forward_type=0;
int ISdetect = 1;
float thresh = 0.53f;
const char* model_dir = nullptr;
const char* input_dir = nullptr;
const char* photo_dir = nullptr;
const char* output_dir= "data/results";

std::ofstream ofs("./data/debug/faceTime.csv", std::ios::out);
std::ofstream txt("./data/debug/faceFeature.txt", std::ios::out);

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

    struct Handle* handle = GetExtractHandle();
    int ret = ExtractInit(handle, model_dir, forward_type, model_type);
    if (ret != 0) {printf("######ret: %d \n", ret); return;}

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

            tiorb_image_feats_info feats_info;
            TIME startTime = utils::GetCurrentTime();
            int ret = ExtractImagePath(handle, fullname.c_str(), &feats_info, photo_dir);
            int64_t endTime = utils::GetDurationTime(startTime);
            if (ret != 0) continue;
            
            if (cout==0) ofs<<fullname<<","<<endTime*0.08f<<","<<endTime*0.05f<<","<<endTime*0.15f<<","<<endTime*0.72f<<","<<endTime<<"\n";

            num_faces = feats_info.num_face;
            if (num_faces == 0) continue;
            float* FaceFeature = feats_info.FaceFeature;
            int* location = feats_info.location;
            for (int i=0; i<num_faces; i++){
                file_vector.push_back(fullname);
                for (int n=0; n<14; n++) rect_vector.push_back(location[i*14+n]); 
                std::vector<float> featvector;
                for (int j = 0; j < 512; j++) {
                    featvector.push_back(*(FaceFeature + i*512 + j));
                    txt<<*(FaceFeature + i*512 + j)<<" ";
                }
                txt<<fullname<<"\n";
                feat_vector.push_back(featvector);
            }
            ExtractDestroyStruct(&feats_info);
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
                FaceCluster(PREFaceNums, clusterfeats, &cluster_info, thresh);
		delete[] clusterfeats;
                num_cluster = cluster_info.num_cluster;
                if (num_cluster == 0) continue;
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
                        utils::CopyFile(file_vector[i], utils::JoinPaths(imagedir,filename));
                        //int rect[4]; for (int j=i*14;j<i*14+4;j++) rect[j-i*14]=rect_vector[j]; 
                        //DrawRectangle(file_vector[i].c_str(), rect, utils::JoinPaths(imagedir,filename+".jpg").c_str());
                        
                        //int FaceRect[14]; for (int j=i*14;j<i*14+14;j++) FaceRect[j-i*14]=rect_vector[j]; 
                        //tiorb_face_photo_info photo_info;
                        //PhotoExtractPath(file_vector[i].c_str(), FaceRect, &photo_info);
                        //PhotoDestroyStruct(&photo_info);
                    } 
                }
                std::vector<std::string>().swap(file_vector);
		std::vector<int>().swap(rect_vector);
            }else{
		float* averagefeats = new float[num_cluster*512];
                int* centerlabels = new int[num_cluster];
                int* labelnums = new int[num_cluster];
                // 获取中间向量
                for (int i = 0; i < num_cluster; ++i) {
                    for (int j = 0; j < 512; ++j) {
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

		int preface_ret = GrowthCluster(num_cluster, averagefeats, centerlabels, labelnums, PREFaceNums, clusterfeats, &cluster_info, thresh);
                
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
                        utils::CopyFile(file_vector[i-num_cluster], utils::JoinPaths(imagedir,filename));
                        //int rect[4]; for (int j=n*4;j<n*4+4;j++) rect[j-n*4]=rect_vector[j]; 
                        //DrawRectangle(file_vector[i-num_cluster].c_str(), rect, utils::JoinPaths(imagedir,filename+".jpg").c_str());
                    }
                }
                num_cluster = cluster_info.num_cluster;
		std::vector<std::string>().swap(file_vector);
		std::vector<int>().swap(rect_vector);
            }
            PREFaceNums = 0;
        }
    } 
    ExtractDestroyModel(handle); 
}


int main(int argc, char **argv){
    int num_cluster=0;
    int PearPREFace=atoi(argv[1]);
    int sum = atoi(argv[2]);
    forward_type = atoi(argv[3]);
    model_dir = argv[4];
    input_dir = argv[5];
    photo_dir = argv[6];
    thresh = atof(argv[7]);
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
    ofs.close(); txt.close();
    ClusterDestroy(&cluster_info);
    return 0;
}







