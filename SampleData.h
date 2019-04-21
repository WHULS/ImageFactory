#ifndef SAMPLEDATA_H
#define SAMPLEDATA_H

#include <QMainWindow>
// 引入OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

// 引入 Qt
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QListView>
#include <QStandardItemModel>
#include <QDebug>
#include <QAbstractItemView>
#include <QKeyEvent>
// xml
#include <QDomDocument>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

#include "CaliImage.h"
#include "Dialogs/ControlPointDlg.h"
#include "Dialogs/SelectCpDlg.h"
#include "Matrix.h" // 矩阵及运算

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

    // 显示控制点图像
    void showControlPoint(Mat cPtImage, vector<CPoint> points, String winName="Control Points");

    // 显示当前图像并设置事件
    void showCurrentImage();

    // 显示提示信息
    int showMessage(QString, QString header=tr("提示"));

public slots:
    void dataListClicked(QModelIndex index);

public:
    void dataListClicked(int row);
    void caliImageChanged(int row, int col=0);

public slots:
    void on_read_control_point_triggered();

    void on_open_image_triggered();

    void on_dataInfo_clicked(const QModelIndex &index);
    void on_dataInfo_activated(const QModelIndex &index);

    void on_save_calibration_info_triggered();

private:
    Ui::SampleData *ui;
    QStandardItemModel *dataListModel; // 数据列表
    QStandardItemModel *dataInfoModel; // 数据内容
    // 图像路径
    QString imageDir = "E:/杉/文章/大三下/4. 计算机视觉/CV实习数据/90";
    QString imagePath;
    // 控制点
    QString cpPath = "E:/杉/文章/大三下/4. 计算机视觉/CV实习数据";

    // XML
    QString xmlDir = "E:/Qt/Program/ImageFactory/Calibration Information";

    // 二维DLT参数（多张影像）
    vector<vector<double>> H;
public:
    Mat currentImage;
    CaliImage caliImage;
    vector<CaliImage> caliImages; // 存储所有检校照片的数组
    vector<CPoint> controlPoints;
    ControlPointDlg *CpDlg;

    int currentCPtNum=0;   // 用来记录当前输入的控制点的号数
    int imageIndex=-1;     // 从数组中取得的照片的编号，只有从数组中取得照片，该编号才会生效，用于更新控制点选择信息
    int currentListNum;    // 记录当前列表内容

private slots:
    void on_open_calibration_info_triggered();
    void on_calculate_dlt_param_triggered();
    void on_orientation_element_initial_value_triggered();
};

#endif // SAMPLEDATA_H
