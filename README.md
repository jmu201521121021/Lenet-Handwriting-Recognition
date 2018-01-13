# 一.部署流程

  - [先参考上一篇博客,学会Lenet在caffe上训练](http://blog.csdn.net/jmu201521121021/article/details/78792268) 
  - [caffe+qt+lenet实现手写体识别源码下载地址](https://github.com/jmu201521121021/Lenet-Handwriting-Recognition/).
  - 把训练好的权重文件和深度神经网络部署到Qt上,实现手写体的识别.
  - 环境要求(*如果未安装请参考我的其他博客,有具体介绍)
      - Ubuntu
      - openCV
      - QtCreator
      - caffe

  - 部署流程图
```flow
st=>start: 开始
e=>end: 结束
op=>operation: 点击选择图片按钮,读取一张图片
op1=>operation: 图片灰度化处理,设置网络的输入大小.
op2=>operation: 图片输入Lenet深度卷积神经网络
op3=>operation: 获得softMax层的值
op4=>operation: 获得softMax的最大值下标
st->op->op1->op2->op3->op4->e
```
- 简单界面设计
  ![这里写图片描述](http://img.blog.csdn.net/20180112214859670?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvam11MjAxNTIxMTIxMDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
  #代码详细介绍
- 代码文件
  - Lenet-5.pro
  - lenet.h
  - mainwindow.h
  - lenet.cpp
  - main.cpp
  - mainwindow.cpp
  - mainwindow.ui
- Lenet-5.pro,配置文件.添加opencv的库目录和caffe库目录及相关依赖项
```
#-------------------------------------------------
#
# Project created by QtCreator 2018-01-11T23:43:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lenet-5
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lenet.cpp

HEADERS  += mainwindow.h \
    lenet.h

FORMS    += mainwindow.ui

#下面代码为具体配置,路径根据自己的修改
INCLUDEPATH += /usr/local/include \
/usr/local/include/opencv \
/usr/local/include/opencv2
LIBS += /usr/local/lib/libopencv_calib3d.so \
/usr/local/lib/libopencv_core.so \
/usr/local/lib/libopencv_features2d.so \
/usr/local/lib/libopencv_flann.so \
/usr/local/lib/libopencv_highgui.so \
/usr/local/lib/libopencv_imgcodecs.so \
/usr/local/lib/libopencv_imgproc.so \
/usr/local/lib/libopencv_ml.so \
/usr/local/lib/libopencv_objdetect.so \
/usr/local/lib/libopencv_photo.so \
/usr/local/lib/libopencv_shape.so \
/usr/local/lib/libopencv_stitching.so \
/usr/local/lib/libopencv_superres.so \
/usr/local/lib/libopencv_videoio.so \
/usr/local/lib/libopencv_video.so \
/usr/local/lib/libopencv_videostab.so
# caffe
INCLUDEPATH += ../include/caffe/include\
../include/caffe/src \

LIBS += /home/jinshan/caffe/build/lib/libcaffe.so
LIBS +=../lib/libcaffe.so.1.0.0-rc3
LIBS += -L../lib
LIBS += -lglog -lgflags -lprotobuf -lboost_system -lboost_thread -llmdb -lleveldb -lstdc++ -lcblas -latlas

QMAKE_CXXFLAGS += -std=c++0x
CONFIG   += c++11

```
- lenet.h.创建一个lenet类,包含网络的具体调用和数据预处理(*核心代码*)
  - Lenet(std::string model_file,std::string delay_file)://初始化网络,加载权重
  - void WrapInputLayer(const vector<cv::Mat> imgs, std::vector<cv::Mat> *input_channels); //多张图片,h*w*3->3*h*w
       void WrapInputLayer(const cv::Mat imgs, std::vector<cv::Mat> *input_channels); //单张图片,h*w*3->3*h*w
    - int Predict(cv::Mat img);          //返回预测结果
    - std::shared_ptr< Net<float>>net; //定义网络
```
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
```
- lenet.cpp

```
#include "lenet.h"

Lenet::Lenet(){}

Lenet::~Lenet(){}

Lenet::Lenet(std::string model_file,std::string delay_file)
{
    //CPU模式
    #ifdef CPU_ONLY
        Caffe::set_mode(Caffe::CPU);
    //GPU模式
    #else
        Caffe::set_mode(Caffe::GPU);
    #endif
    net.reset(new Net<float>(delay_file,TEST)); //加载网络文件
    net->CopyTrainedLayersFrom(model_file);     //加载权重文件
}
//将w*h*3转化位3*h*w
void Lenet:: WrapInputLayer(const vector<cv::Mat> imgs, std::vector<cv::Mat> *input_channels)
{
    Blob<float> *input_layer = net->input_blobs()[0]; //取数据层blob
    int width = input_layer->width();                 //获得blob的宽度
    int height = input_layer->height();               //获得blob高度
    int num = input_layer->num();                     //blob数量
    float *input_data = input_layer->mutable_cpu_data();    //指针方向指向数据blob
    for (int j = 0; j < num; j++) { //input_data ->input_channels;
        //std::vector<cv::Mat> *input_channels;
        for (int k = 0; k < input_layer->channels(); ++k)
        {
            cv::Mat channel(height, width, CV_32FC1, input_data);
            input_channels->push_back(channel);
            input_data += width * height;
        }
        cv::Mat img = imgs[j];
        cv::split(img, *input_channels);//赋值->给网络赋值,分为三通道:r,g,b
        input_channels->clear();
    }
}

//单张图片,h*w*3->3*h*w
 void Lenet::WrapInputLayer(const cv::Mat imgs, std::vector<cv::Mat> *input_channels)
 {
     Blob<float> *input_layer = net->input_blobs()[0]; //取数据层blob
     int width = input_layer->width();                 //获得blob的宽度
     int height = input_layer->height();               //获得blob高度
     float *input_data = input_layer->mutable_cpu_data();    //指针方向指向数据blob
     for (int k = 0; k < input_layer->channels(); ++k)
     {
             cv::Mat channel(height, width, CV_32FC1, input_data);
             input_channels->push_back(channel);
             input_data += width * height;
     }
     cv::split(imgs, *input_channels);//赋值->给网络赋值,分为三通道:r,g,b

 }

  //预测结果
 int Lenet::Predict(cv::Mat img)
 {
     std::vector<float> softMax;
     cv::Mat grayImg;
     if(img.channels()==3)
     {
         cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
     }
     //输入必须位灰度图
     cv::resize(grayImg,grayImg, cv::Size(28,28),cv::INTER_CUBIC); //输入为28*28
     //cv::imwrite("w.jpg", grayImg);
     grayImg.convertTo(grayImg,CV_32FC1, 0.00392157);              //归一化 

     //rehape net 设置网络输入尺寸
     Blob<float> *input_layer = net->input_blobs()[0];
     input_layer->Reshape(1, 1,grayImg.rows, grayImg.cols);
     net->Reshape();

     //change channels 数据转换
     std::vector<cv::Mat>channels;
     WrapInputLayer(grayImg,&channels);

     //forword net 前向传播
     net->Forward();

     //get SOftMax label 取网络最后一层SoftMax
     Blob<float>* out_layer = net->output_blobs()[0];
     int count = out_layer->count();
     const float* start_feature = out_layer->cpu_data();
     const float* end_feature = out_layer->cpu_data() + count;
     softMax =std::vector<float>(start_feature, end_feature);

     //获得最大值的下标index=预测数字
     float result = softMax[0];
     int index = 0;
     cout<< "sfds"<<softMax.size()<<endl;
     for(int i =  1; i < softMax.size();i++)
     {
         if(result< softMax[i])
         {
             result = softMax[i];
             index = i;
         }
         cout<<i<<":"<<softMax[i];
     }
     cout<<"result:"<<index<<" ";

     return index;
 }
```
- mainwindow.cpp,网络调用和初始化,选择图片进行处理得出预测

```
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lenet.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置窗口
    this->showMaximized(); //最大化
    ui->srcImage->setScaledContents(true); //图片自适应label
    //初始化网络
    string model_file = "./lenet_iter_10000.caffemodel"; //权重文件
    string delay_file = "./lenet_delay.prototxt";       //网络文件
    lenetCNN = Lenet(model_file,delay_file);            //初始化
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_selectImage_clicked() //按钮selectImage的槽函数
{
    //获得图片路径
    QString fileName = QFileDialog::getOpenFileName(
                this, "open image",QDir::currentPath(),
                "All files(*.jpg *.png *.bmp)"
    );

    //std::cout<<fileName.toStdString();
    cv::Mat img; //定义图片
    int resultNumber; //识别结果
    img = cv::imread(fileName.toStdString()); //读取图片

    resultNumber = lenetCNN.Predict(img);   //预测

    ui->reseult->setText("number: " + QString::number(resultNumber)); //显示
   // cout<<"number:"<<resultNumber;
    //图片显示到srcImage控件上
    cv::cvtColor(img, img, CV_BGR2RGB);
    QImage qImg = QImage((const unsigned char*)(img.data), img.cols, img.rows, img.cols * img.channels(), QImage::Format_RGB888);
    ui->srcImage->clear();
    ui->srcImage->setPixmap(QPixmap::fromImage(qImg));
}
```
- main.cpp 没有改动
```
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

```

- mainwindow.ui是界面文件,自行下载源码查看