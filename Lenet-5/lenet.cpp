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
