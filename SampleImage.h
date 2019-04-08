#ifndef SAMPLEIMAGE_H
#define SAMPLEIMAGE_H

// 引入OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

// 定义结构体存储控制点
/**
  * @param x
  * @param y
  * @param z
  * @param num 点号
  */
typedef struct {
    double x;
    double y;
    double z;
    int num;
} CPoint;

class SampleImage
{
private:
    // 存储控制点信息的数组
    vector<CPoint> controlPoints;

    // 存储样本照片的数组
    vector<Mat> sampleImages;
public:
    SampleImage();

    // 判断控制点是否为空
    bool controlPointEmpty();
	// 判断样本照片是否为空
    bool sampleImageEmpty();

    // 添加控制点，返回添加的位置
    size_t pushControlPoint(double x, double y, double z, int num);
	size_t pushControlPoint(CPoint cp);
	// 添加影像，返回添加的位置
    size_t pushSampleImage(Mat img);

    // 获取标号为num的控制点
    bool getControlPoint(int num, CPoint *cp);
	// 获取数组中第num幅影像
    bool getSampleImage(size_t num, Mat *img);

	// 获取控制点数量
    int controlPointNumber();
	// 获取影像数量
    int sampleImageNumber();

	// 清空控制点
	bool clearControlPoint();
	// 清空影像
	bool clearSampleImage();

    // 对指定像片进行检校
    bool calibration(size_t imageNum);

// static 方法
public:
    static void CPointMouseClick(int event, int x, int y, int flags, void* userdata);
};

#endif // SAMPLEIMAGE_H
