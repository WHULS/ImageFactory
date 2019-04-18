#ifndef CALIBRATIONWIN_H
#define CALIBRATIONWIN_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

// STL
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

namespace Ui {
class CalibrationWin;
}

class CalibrationWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalibrationWin(QWidget *parent = nullptr);
    ~CalibrationWin();
    int showMessage(QString, QString header="提示");
    void showImage(Mat img, QString str=tr("Image"));
    void print(Mat mat);
    void print(vector<Mat>);

private slots:
    void on_open_image_triggered();

    void on_calibration_triggered();

private:
    Ui::CalibrationWin *ui;
    Mat image;
    QString imageDir;
};

#endif // CALIBRATIONWIN_H
