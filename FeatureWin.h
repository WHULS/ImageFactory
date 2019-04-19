#ifndef FEATUREWIN_H
#define FEATUREWIN_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QWheelEvent>

// OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;

// STL
#include <vector>
#include <iostream>
using namespace std;

#include "convert.h"

namespace Ui {
class FeatureWin;
}

class FeatureWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit FeatureWin(QWidget *parent = nullptr);
    ~FeatureWin();

    int showMessage(QString);
    void showImage(Mat left, Mat right);
    void showImage(QImage l, QImage r);
    double min(double,double,double,double);
    double max(Mat);

protected:
    void wheelEvent(QWheelEvent *);

private slots:
    void on_open_image_triggered();

    void on_moravec_fetch_triggered();

private:
    Ui::FeatureWin *ui;

    // 影像
    QImage qLeftImage, qRightImage;
    Mat leftImage, rightImage;

    QString imageDir;
    int zoomScale = 100;
};

#endif // FEATUREWIN_H
