#ifndef LENET_H
#define LENET_H

#define CPU_ONLY //CPU Mode

#include <caffe/caffe.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <math.h>
#include <QMessageBox>
#include <qfile.h>
#include <QTextStream>
#include <QStatusBar>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace  std;
using namespace caffe;

class Lenet
{
public:
     Lenet();
    ~Lenet();
    Lenet(std::string model_file,std::string delay_file); //model_file->weight delay->file->netFile
    void WrapInputLayer(const vector<cv::Mat> imgs, std::vector<cv::Mat> *input_channels); //多张图片,h*w*3->3*h*w
    void WrapInputLayer(const cv::Mat imgs, std::vector<cv::Mat> *input_channels); //单张图片,h*w*3->3*h*w
    int Predict(cv::Mat img);          //返回结果
    std::shared_ptr< Net<float>>net; //网络
};

#endif // LENET_H
