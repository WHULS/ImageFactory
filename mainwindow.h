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

private slots:
    void on_image_open_triggered();

    void on_clear_image_triggered();

    void on_edge_canny_triggered();

    void showImage(Mat);
    Mat edgeDetectCanny(Mat, int edgeThresh = 1);

    void on_show_srcImage_triggered();

public slots:
    void On_CannySlider_valueChanged(int); // 自定义信号响应插槽（On大写区分系统定义）

private:
    Ui::MainWindow *ui;

    cv::Mat srcImage;

    QGraphicsScene *scene;
    MyGraphicsView *ImageView;
    MySlider *CannySlider;

};

#endif // MAINWINDOW_H
