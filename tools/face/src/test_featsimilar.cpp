#include "linux_api.h"
#include "utils.h"
#include <iostream>
#include <cstring>

float CalculateSimilarity(const std::vector<float>&feat1, const std::vector<float>& feat2, int featnums) {
    if ((feat1.size() != feat2.size()) || (feat1.size() != featnums))
        return -1;
    float inner_product = 0.0;
    for(int i = 0; i < featnums; ++i) {
        inner_product += feat1[i] * feat2[i];
    }
    return inner_product;
}


void split(string str,string pattern,vector<float>& result1,vector<string>& result2, int featnums)
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
            if (sum <= featnums) {
		result1.push_back(atof(s.c_str()));
	    }else{
		result2.push_back(s);
	    }
            i = pos + pattern.size() - 1;
        }
    }
}

int main(int argc, char **argv) {
    int featnums = atoi(argv[1]);
    const char* feature_txt1 = argv[2];
    const char* feature_txt2 = argv[3];

    std::vector<float>  result1;
    std::vector<float>  result2;
    std::vector<std::string> result3;
    std::ifstream txt1(feature_txt1);
    if (txt1){
        std::string content;
        while (getline(txt1, content, '\n')){
	    split(content, " ", result1, result3, featnums);
        }
    }else{
        std::cout << "Open file1 faild." << std::endl;
    }
    txt1.close();

    std::ifstream txt2(feature_txt2);
    if (txt2){
        std::string content;
        while (getline(txt2, content, '\n')){
	    split(content, " ", result2, result3, featnums);
        }
    }else{
        std::cout << "Open file2 faild." << std::endl;
    }
    txt2.close();
    
    float similar = CalculateSimilarity(result1, result2, featnums);
    std::cout<<"==========>similar:"<<similar<<std::endl;
    return 0;
}







