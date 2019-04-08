#include "SampleImage.h"

// 引入 Qt
#include <QMessageBox>

SampleImage::SampleImage()
{

}

bool SampleImage::controlPointEmpty()
{
    if (controlPoints.empty())
        return true;
    else
        return false;
}
bool SampleImage::sampleImageEmpty()
{
    if (sampleImages.empty())
        return true;
    else
        return false;
}

// 添加控制点，返回添加的位置
size_t SampleImage::pushControlPoint(double x, double y, double z, int num)
{
    CPoint cp = {x, y, z, num};
    controlPoints.push_back(cp);
    return controlPoints.size()-1;
}
size_t SampleImage::pushControlPoint(CPoint cp)
{
    controlPoints.push_back(cp);
    return controlPoints.size()-1;
}
// 添加影像，返回添加的位置
size_t SampleImage::pushSampleImage(Mat img)
{
    sampleImages.push_back(img);
    return sampleImages.size()-1;
}

// 获取标号为num的控制点
bool SampleImage::getControlPoint(int num, CPoint *cp)
{
    if (controlPoints.empty())
    {
		// 控制点为空
		return false;
    }
    else
    {
        for (size_t i=0; i<controlPoints.size()-1; i++)
        {
            if (controlPoints[i].num == num)
            {
                *cp = controlPoints[i];
                return true;
            }
        }
		// 找不到控制点
        return false;
    }
}
// 获取数组中第num幅影像
bool SampleImage::getSampleImage(size_t num, Mat *img)
{
    if (sampleImages.empty())
    {
        return false;
    }
	else
	{
		// 超出图像范围
		if (num > sampleImages.size()-1)
			return false;
		
		*img = sampleImages[num];
		return true;
	}
	
}

// 获取控制点数量
int SampleImage::controlPointNumber()
{
    return int(controlPoints.size());
}
// 获取影像数量
int SampleImage::sampleImageNumber()
{
    return int(sampleImages.size());
}

// 清空控制点
bool SampleImage::clearControlPoint()
{
    controlPoints.clear();
    return true;
}
// 清空影像
bool SampleImage::clearSampleImage()
{
    sampleImages.clear();
    return true;
}

void SampleImage::CPointMouseClick(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONUP)
    {
        QMessageBox::information(nullptr,
                                 QObject::tr("Control Point"),
                                 QString().sprintf("%d\n%d", x, y));
    }
}

// 对指定像片进行检校
bool SampleImage::calibration(size_t imageNum)
{
    Mat caliImage;
    if (getSampleImage(imageNum, &caliImage))
    {
        // 显示照片
        namedWindow("Detect CPoint", WINDOW_NORMAL);
        imshow("Detect CPoint", caliImage);
        
        // 事件监听
        setMouseCallback("Detect CPoint", CPointMouseClick);
        
        return true;
    }
    else
    {
        cout << "像片获取失败" << endl;
        return false;
    }
    
}
