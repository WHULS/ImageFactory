#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QSlider>
#include "mygraphicsview.h"
#include "myslider.h"
#include "SampleData.h"

// 引入OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <math.h>

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

// 边缘提取方法
enum EdgeMethod{mCanny=0, mLaplacian=1, mSobel=2, mLOG=3, mPrewitt=4, mRoberts=5,
               mDOG=6};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void releaseImages();
    void showImage(Mat, const char*);
    void showImage(Mat, bool = false);
    void resizeToImage(Mat); // 调整窗口到最适照片
    Mat edgeDetectCanny(Mat, int edgeThresh = 1);
    void showMessageBox(QString);
    Mat difference_of_gaussian(Mat image, int sigma=3, int ksize=15, int k=3);
    void HoughCircleDetect(Mat image, int method, double dp, double minDist, double param1, double param2, int minRadius, int maxRadius);

private slots:
    void on_image_open_triggered();

    void on_clear_image_triggered();

    void on_edge_canny_triggered();

    void on_show_srcImage_triggered();

    void on_fit_to_image_triggered();

    void on_edge_laplacian_triggered();

    void on_edge_log_triggered();

    void on_edge_sobel_triggered();

    void on_edge_roberts_triggered();

    void on_edge_prewitt_triggered();

    void on_show_edge_triggered();

    void on_show_blur_triggered();

    void on_show_gray_triggered();

    void on_edge_dog_triggered();

    void on_calibration_triggered();

    void on_read_control_point_triggered();

    void on_point_Moravec_triggered();

public slots:
    void On_CannySlider_valueChanged(int); // 自定义信号响应插槽（On大写区分系统定义）
    void On_HoughCircle_valueChanged(double, double, double, double, int, int);

private:
    Ui::MainWindow *ui;

    // 图像
    QString imagePath = "E:/杉/文章/大三下/4. 计算机视觉/CV实习数据/90";
    cv::Mat srcImage;  // 原始图像
    cv::Mat edgeImage; // 边缘图像
    int edgeMethod = -1;
    Mat grayImage; // 灰度图
    Mat blurImage; // 模糊图

    // 屏幕
    int availableHeight;
    int availableWidth;

    QGraphicsScene *scene;
    MyGraphicsView *ImageView;
    MySlider *CannySlider;

    // 存储和处理样本图像的类
    SampleData *sampleData;

    // 控制点
    QString cpPath = "E:/杉/文章/大三下/4. 计算机视觉/CV实习数据";
};

#endif // MAINWINDOW_H
