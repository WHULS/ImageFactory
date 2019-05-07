#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

class Wallis
{
public:
    Wallis(Mat _SourceImg, float _B_Value, float _C_Value, float _meanValue, float _sigmaValue);

    /*bRow、bCol Mask的起始行列号；winRow、winCol Mask的大小*/
    Mat ReadMask(Mat SourceImg,int bRow, int bCol, int winRow, int winCol);

    /*grid计算出的r0或r1格网；gridRow、gridCol格网大小；*value插值结果；x、y需要进行插值计算的位置；*/
    int	InterplotWallisParameter(float *grid, int gridRow, int gridCol,float *value, int x, int y);
    /*Mask进行Wallis滤波处理的影像区域；winRow、winCol影像区域大小；r0、r1 Wallis滤波系数*/
    int	CalWallisParameter(Mat Mask, int winRow, int winCol, float *r0, float *r1);

    void Wallisfilter(Mat Result);
    ~Wallis();
private:
    Mat SourceImg;   // 要处理的影像
    float B_Value;   // 影像反差扩展常数
    float C_Value;   // 影像亮度系数
    float meanValue; // 均值目标值
    float sigmaValue;// 方差目标值
};
