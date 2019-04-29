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
#include <opencv2/stitching.hpp>
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

typedef struct {
    Point lpt;
    Point rpt;
    double cVal;
} MatchPoint;

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
    void showMergeImage(Mat left, Mat right);
    // 绘制所有匹配线
    void showMatchImage(Mat left, Mat right, vector<MatchPoint> matchPoints);
    // 绘制一条匹配线
    void showMatchImage(Mat left, Mat right, MatchPoint point);
    double min(double,double,double,double);
    void merge(Mat m1, Mat m2, Mat &merged);

    void moravec(Mat image, Mat &out, int factorSize=9, int searchAreaSize=9);
    void forstner(Mat image, Mat &out, int factorSize, int searchAreaSize, double Tq=0.5, double f=0.75);
    void harris(Mat image, Mat &out, int blurRadius=9, double sigma=1.5, double qualityLevel=0.01);
    double correlation(Mat win1, Mat win2);
    void drawCorner(Mat &input, Mat corner, int thickness=1, int radius=1, Scalar color=Scalar(0, 255, 0));

    void initTable();
    void createTable(QStringList strList);
    void presentMatchInfo(vector<MatchPoint>);

public slots:
    void moravecChanged(int factorSize, int searchAreaSize);
    void forstnerChanged(int factorSize, int searchAreaSize, double Tq, double f);
    void showPoint(QModelIndex idx);

protected:
    void wheelEvent(QWheelEvent *);

private slots:
    void on_open_image_triggered();

    void on_moravec_fetch_triggered();

    void on_forstner_fetch_triggered();

    void on_harris_fetch_triggered();

    void on_correlation_index_triggered();

    void on_least_square_triggered();

private:
    Ui::FeatureWin *ui;

    // 影像
    QImage qLeftImage, qRightImage;
    Mat leftImage, rightImage;
    Mat leftCornerImage, rightCornerImage;

    // 角点
    Mat leftCorner, rightCorner;
    vector<MatchPoint> matchPoints;

    QString imageDir;
    int zoomScale = 100;

    // 数据表
    QStandardItemModel *pointInfoModel;
};

#endif // FEATUREWIN_H
