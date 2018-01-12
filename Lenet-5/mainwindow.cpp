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

void MainWindow::on_selectImage_clicked()
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
