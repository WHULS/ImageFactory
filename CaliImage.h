#ifndef CALIIMAGE_H
#define CALIIMAGE_H

// 引入Qt
#include <QString>

// 引入 OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// 存储控制点的结构体
/**
  * @param x
  * @param y
  * @param z
  * @param num 点号
  */
typedef struct {
	// 世界坐标系坐标
	double X; // mm
	double Y; // mm
	double Z; // mm

	int num;

	// 图像坐标系坐标
	double x; // mm
	double y; // mm
} CPoint;

class CaliImage
{
public:
	// 方法
	CaliImage();
	~CaliImage();
    void clear();

	// 变量
    QString ImagePath;
	Mat Image;
	vector<CPoint> ControlPoints; // 影像对应的控制点
	double X; // mm
	double Y; // mm
	double Z; // mm
	double Phi;   // rad
	double Omega; // rad
	double Kappa; // rad
	double x_0; // mm
	double y_0; // mm
	double f;   // mm
};

#endif // CALIIMAGE_H
