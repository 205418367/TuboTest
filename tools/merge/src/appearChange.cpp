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
    const char *json_path = argv[2]; //"data/cluster";
    const char *vedio_dir = argv[3]; //"data/results";
   
    int ret = AppearChangeInit(model_dir, 0);
    if (ret != 0) {
        printf("appear init failed %d \n", ret);
        exit(1);
    }

    std::ifstream jsonfile(json_path);
    nlohmann::json data = nlohmann::json::parse(jsonfile);

    TiorbVideoAppearInfo info;
    ret = AppearChangeThread(data.dump().c_str(), vedio_dir, &info, 30);
    if (ret != 0) {
        printf("appear change failed %d \n", ret);
        exit(1);
    }
    AppearChangeDestroy(&info);
    return 0;
}

