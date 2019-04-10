#ifndef SAMPLEIMAGE_H
#define SAMPLEIMAGE_H

// 引入OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

// 引入 Qt
#include <QMessageBox>
#include <QString>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

#include "CaliImage.h"
#include "ControlPointDlg.h"
#include "SelectCpDlg.h"

class SampleData
{
public:
    SampleData();
    ~SampleData();

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

    // 对指定像片进行检校
    bool calibration(size_t imageNum);
    bool calibration(QString imagePath);

    void showCpDlg();
};

#endif // SAMPLEIMAGE_H
