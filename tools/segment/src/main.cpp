#include "LIN_MODNet.h"
#include "utils.h"
#include "linux_api.h"
    
int main(int argc, char **argv) {  
    const char *model_dir = argv[1];
    const char *image_dir = argv[2];
    const char *bgPath = argv[3];
    const char *outpt_dir = argv[4];

    int ret = InitParams(model_dir, "ee41748965094fc6", "6d61d890892af4ed2211381db9ceeea2");
    if (ret != 0) std::cout<<"====>InitParams:"<<ret<<std::endl;

    struct Handle* handle = HSegmentHandle();
    ret = HSegmentInit(handle, model_dir, 0);
    //if (ret != 0) goto exit; 

    vector<string> imgVec = utils::TraverseDirectory(image_dir);
    for (auto path : imgVec){
	std::cout<< "path:<"<<path <<">"<<std::endl;
	const auto &start1 = std::chrono::steady_clock::now();
	TiorbHumanSegmentInfo info;
	ret = HSegmentInferPath(handle, path.c_str(), bgPath, &info);
	const auto &end1 = std::chrono::steady_clock::now();
	if (ret != 0) continue;
	std::cout<<"#### infer time:"<<(end1 - start1).count() / 1000000.0 <<"ms"<<std::endl;

        string _, name; utils::splitPathAndName(path, _, name);
	std::string outpath = utils::JoinPaths(outpt_dir, name);
        utils::saveImage(info.img, info.img_size, outpath+".jpg");
    }
     
//exit:
    //std::cout<<"====>ret:"<<ret<<std::endl;
    return 0;  
}
