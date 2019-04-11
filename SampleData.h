#ifndef SAMPLEDATA_H
#define SAMPLEDATA_H

#include <QMainWindow>
// 引入OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

// 引入 Qt
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QListView>
#include <QStandardItemModel>
#include <QDebug>
#include <QAbstractItemView>
// xml
#include <QDomDocument>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

#include "CaliImage.h"
#include "ControlPointDlg.h"
#include "SelectCpDlg.h"

namespace Ui {
class SampleData;
}

class SampleData : public QMainWindow
{
    Q_OBJECT

public:
    explicit SampleData(QWidget *parent = nullptr);
    ~SampleData();
public:
    // 判断控制点是否为空
    bool controlPointEmpty();
    // 判断样本照片是否为空
    bool caliImageEmpty();

    // 添加控制点，返回添加的位置
    size_t pushControlPoint(double x, double y, double z, int num);
    size_t pushControlPoint(CPoint cp);
    // 添加影像，返回添加的位置
    size_t pushSampleData(CaliImage img);

    // 获取标号为num的控制点
    bool getControlPoint(int num, CPoint *cp);
    // 获取数组中第num幅影像
    bool getSampleData(size_t num, Mat *img);

    // 获取控制点数量
    int controlPointNumber();
    // 获取影像数量
    int caliImageNumber();

    // 清空控制点
    bool clearControlPoint();
    // 清空影像
    bool clearSampleData();

    void showCpDlg();

    void renewData(int idx);

    // 绘制十字
    void drawCross(Mat img, Point center, int size=100, int thickness=1,  Scalar color=Scalar(0,0,255));
    // 在指定区域检测椭圆
    bool detectEllipse(Mat roiImg);

public slots:
    void dataListClicked(QModelIndex index);
public:
    void dataListClicked(int row);

public slots:
    void on_read_control_point_triggered();

    void on_open_image_triggered();

    void on_dataInfo_clicked(const QModelIndex &index);

    void on_save_calibration_info_triggered();

private:
    Ui::SampleData *ui;
    QStandardItemModel *dataListModel; // 数据列表
    QStandardItemModel *dataInfoModel; // 数据内容
    // 图像路径
    QString imageDir = "F:/杉/文章/大三下/4. 计算机视觉/CV实习数据/90";
    QString imagePath;
    // 控制点
    QString cpPath = "F:/杉/文章/大三下/4. 计算机视觉/CV实习数据";
    int currentCPtNum=0; // 用来记录当前输入的控制点的号数

    // XML
    QString xmlDir = "D:/";
public:
    Mat currentImage;
    CaliImage caliImage;
    vector<CaliImage> caliImages; // 存储所有检校照片的数组
    vector<CPoint> controlPoints;
    ControlPointDlg *CpDlg;
};

#endif // SAMPLEDATA_H
