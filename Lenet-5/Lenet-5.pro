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
#LIBS += -L/home/jinshan/caffe/build/lib
#LIBS += -lcaffe
LIBS += /home/jinshan/caffe/build/lib/libcaffe.so
LIBS +=../lib/libcaffe.so.1.0.0-rc3

# cuda
#INCLUDEPATH += /usr/local/cuda/include \
#LIBS += -L/usr/local/cuda/lib64
#LIBS += -lcudart -lcublas -lcurand

# other dependencies
LIBS += -L../lib
LIBS += -lglog -lgflags -lprotobuf -lboost_system -lboost_thread -llmdb -lleveldb -lstdc++ -lcblas -latlas

QMAKE_CXXFLAGS += -std=c++0x
CONFIG   += c++11
