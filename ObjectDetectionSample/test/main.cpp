#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#if 0
#include "core.hpp"
#include "opencv.hpp"
#include "imgproc.hpp"
#include "imgproc/imgproc.hpp"
#endif
#if 1
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#endif
#include "ObjectDetection.h"
#include "Log.h"

int main(int argc, char** argv) {

    std::string* labels = new std::string[1001];
    std::ifstream in("../test/imagenet_slim_labels.txt");
    std::string line;
    int count=0;
    while (getline(in,line)){
        labels[count] = line;
        count++;
    }
    ObjectDetection* detector = new ObjectDetection();
    detector->init(GPU);
    detector->setConfidence(0.1f);

    cv::Mat img = cv::imread("../test/orange.jpeg");
    if(img.empty()){
        std::cout<<"empty"<<std::endl;
    }
    //test
    std::vector<int> result = detector->doDetect(img);
    std::cout<<"data size: "<< result.size()<<std::endl;
    if(result.size()>0){
        for(int i =0;i< result.size();i++){
            std::cout<<"name : "<< labels[result[i]]<<std::endl;
        }
    }
    return 0;
}
