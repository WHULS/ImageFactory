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
#include <QTime>
#include <QStandardItemModel>

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
#include "Matrix.h"
#include "Dialogs/MoravecDlg.h"
#include "Dialogs/ForstnerDlg.h"

namespace Ui {
class FeatureWin;
}

class FeatureWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit FeatureWin(QWidget *parent = nullptr);
    ~FeatureWin();

    const double PI = 3.1415926;

    int showMessage(QString);
    void showImage(Mat image);
    void showImage(Mat left, Mat right);
    void showImage(QImage l, QImage r);
    double min(double,double,double,double);

    void moravec(Mat image, int factorSize=9, int searchAreaSize=9);
    void forstner(Mat image, int factorSize, int searchAreaSize, double Tq=0.5, double f=0.75);
    void harris(Mat image, Mat &out, int blurRadius=9, double sigma=1.5, double qualityLevel=0.01);
    double correlation(Mat win1, Mat win2);

    void initTable();
    void createTable(QStringList strList);

    Matrix getGaussianFunction(int blurRadius, double sigma);
public slots:
    void moravecChanged(int factorSize, int searchAreaSize);
    void forstnerChanged(int factorSize, int searchAreaSize, double Tq, double f);

protected:
    void wheelEvent(QWheelEvent *);

private slots:
    void on_open_image_triggered();

    void on_moravec_fetch_triggered();

    void on_forstner_fetch_triggered();

    void on_harris_fetch_triggered();

    void on_correlation_index_triggered();

private:
    Ui::FeatureWin *ui;

    // 影像
    QImage qLeftImage, qRightImage;
    Mat leftImage, rightImage;

    // 角点
    Mat leftCorner, rightCorner;

    QString imageDir;
    int zoomScale = 100;

    // 数据表
    QStandardItemModel *pointInfoModel;
};

#endif // FEATUREWIN_H
