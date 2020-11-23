//
// Created by liqing on 18-12-3.
//

#include "ObjectDetection.h"
#include "MyUdlLayers.h"
#include "snpe_udl_layers.h"
#include "TimeUtil.h"

#include <DlContainer/IDlContainer.hpp>
#include <DlSystem/String.hpp>
#include <DlSystem/DlError.hpp>
#include <DlSystem/ITensor.hpp>
#include <DlSystem/ITensorFactory.hpp>
#include <SNPE/SNPEFactory.hpp>
#include <SNPE/SNPEBuilder.hpp>

#define USE_MODEL_FILE
#ifdef USE_MODEL_FILE
#define NIKE_MODEL_PATH "../models/inception_v3.dlc"
#endif

#define MODEL_INPUT_W 299
#define MODEL_INPUT_H 299
#define MODEL_INPUT_C 3

#include <iostream>
std::string float2str(float number){
    std::ostringstream buff;
    buff << std::setiosflags(std::ios::fixed) << std::setprecision(4);
    buff << number;
    return buff.str();
}

ObjectDetection::ObjectDetection():mConfidenceThreshold(0.1f) {
}

ObjectDetection::~ObjectDetection() {
    if (snpe != nullptr) {
        snpe.reset(nullptr);
    }
}

state_t ObjectDetection::init(int device) {
    std::unique_ptr<zdl::DlContainer::IDlContainer> container;
#ifdef USE_MODEL_FILE
    container = zdl::DlContainer::IDlContainer::open(zdl::DlSystem::String(NIKE_MODEL_PATH));
#endif
    zdl::SNPE::SNPEBuilder snpeBuilder(container.get());


    zdl::DlSystem::Runtime_t runtime;
    switch (device) {
        case CPU: runtime = zdl::DlSystem::Runtime_t::CPU;break;
        case GPU: runtime = zdl::DlSystem::Runtime_t::GPU;break;
        case DSP: runtime = zdl::DlSystem::Runtime_t::DSP;break;
        case APU: runtime = zdl::DlSystem::Runtime_t::AIP_FIXED8_TF; break;
        default:  runtime = zdl::DlSystem::Runtime_t::GPU;break;
    }

    zdl::DlSystem::UDLBundle udlBundle;
    zdl::DlSystem::PerformanceProfile_t profile = zdl::DlSystem::PerformanceProfile_t::HIGH_PERFORMANCE;

    snpe = snpeBuilder.setOutputLayers(outputLayers)
            .setRuntimeProcessor(runtime)
            .setCPUFallbackMode(true)
            .setPerformanceProfile(profile)
            .setUdlBundle(udlBundle)
            .build();
    if(nullptr == snpe) {
        const char* const err = zdl::DlSystem::getLastErrorString();
        std::cout<<"!!!!!! ERROR code :"<<err<<std::endl;
    } else {

        const auto strList = snpe->getInputTensorNames();
        auto inputDims = snpe->getInputDimensions((*strList).at(0));
        const zdl::DlSystem::TensorShape& inputShape = *inputDims;
        size_t rank = inputShape.rank();
        int input_size = 1;
        for (size_t i=0; i<rank; i++) {
            input_size *= inputShape[i];
            std::cout<<"input shape"<<i<<":"<<inputShape[i]<<std::endl;
        }

        inTensor = zdl::SNPE::SNPEFactory::getTensorFactory().createTensor(
                inputDims
        );
        inMap.add((*strList).at(0), inTensor.get());
        std::cout<<"snpe init finish"<<std::endl;
        zdl::DlSystem::Version_t Version = zdl::SNPE::SNPEFactory::getLibraryVersion();
    }
    return NO_ERROR;
}

state_t ObjectDetection::deInit() {
    if (nullptr != snpe) {
        snpe.reset(nullptr);
    }
    return NO_ERROR;
}

void ObjectDetection::setConfidence(float value) {
    mConfidenceThreshold = value > 1.0f ? 1.0f : (value < 0.0f ? 0.0f : value);
}

std::vector<int> ObjectDetection::doDetect(cv::Mat img) {
    std::vector<int> result;
    if (NULL == snpe) {
        //TS_LOGE("can not init err!");
        return result;
    }
    cv::Mat input;
    long start_pre = getCurrentTime_ms();

    // cv resize
#if 1
    cv::Mat resize_mat;
    cv::resize(img, resize_mat, cv::Size(299, 299));
    cv::cvtColor(resize_mat, input, CV_BGR2RGB);

    cv::Mat input_norm(MODEL_INPUT_H, MODEL_INPUT_H, CV_32FC3, inTensor.get()->begin().dataPointer());
    input.convertTo(input, CV_32F);
    cv::normalize(input, input_norm, -1.0f, 1.0f, cv::NORM_MINMAX);
#endif


    long diff_pre = getCurrentTime_ms() - start_pre;
    zdl::DlSystem::ITensor* outputTensor = nullptr;
    long start = getCurrentTime_ms();
    bool ret = snpe->execute(inMap, outMap);
    long diff = getCurrentTime_ms() - start;
    if (!ret) {
        const char* const err = zdl::DlSystem::getLastErrorString();
        std::cout<<"!!!!!!ERROR code:"<<err<<std::endl;
        return result;
    }
    zdl::DlSystem::StringList tensorNames = outMap.getTensorNames();
    for( auto& name: tensorNames ){
        std::cout<< "tensor name: "<<name<<std::endl;
        outputTensor = outMap.getTensor(name);
    }
    zdl::DlSystem::TensorShape shape = outputTensor->getShape();

    size_t rank = shape.rank();
    int input_size = 1;
    for (size_t i=0; i<rank; i++) {
        input_size *= shape[i];
        std::cout<<"output shape"<<i<<":"<<shape[i]<<std::endl;
    }
    const float* final_result = &*(outputTensor->cbegin());

    for(int i =0; i<shape[1]; i++){
        float confidence = *final_result++;

        if(confidence>mConfidenceThreshold){
            result.push_back(i);
            std::cout<<"i: "<<i<<std::endl;
            std::cout<<"confidence: "<< confidence<<std::endl;
        }
    }

    long start_post = getCurrentTime_ms();
    outMap.clear();
    long diff_post = getCurrentTime_ms() - start_post;
    long diff_sum = getCurrentTime_ms() - start_pre;
    std::cout<<"sum cost time="<<diff_sum<<std::endl;

    return result;
}