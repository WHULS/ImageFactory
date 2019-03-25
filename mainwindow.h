#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QSlider>
#include "mygraphicsview.h"
#include "myslider.h"

// 引入OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void releaseImages();

    void showImage(Mat, bool = true);

    void resizeToImage(Mat); // 调整窗口到最适照片

    Mat edgeDetectCanny(Mat, int edgeThresh = 1);

    void showMessageBox(QString);

private slots:
    void on_image_open_triggered();

    void on_clear_image_triggered();

    void on_edge_canny_triggered();

    void on_show_srcImage_triggered();

    void on_fit_to_image_triggered();

public slots:
    void On_CannySlider_valueChanged(int); // 自定义信号响应插槽（On大写区分系统定义）

private:
    Ui::MainWindow *ui;

    // 图像
    cv::Mat srcImage; // 原始图像
    cv::Mat cedge; // Canny 边缘图像

    // 屏幕
    int availableHeight;
    int availableWidth;

    QGraphicsScene *scene;
    MyGraphicsView *ImageView;
    MySlider *CannySlider;

};

#endif // MAINWINDOW_H
