#include "kinectwindow.h"
#include "ui_kinectwindow.h"

#include <QMessageBox>

KinectWindow::KinectWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KinectWindow)
{
    ui->setupUi(this);
}

KinectWindow::~KinectWindow()
{
    delete ui;
}

// 初始化 Kinect
bool KinectWindow::init()
{
    bool kinect = true;
    if (!kinect)
    {
        return false;
    }
    else
    {
        return true;
    }
}
