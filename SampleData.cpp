#include "SampleData.h"

// 声明静态成员
static Mat currentImage;
static vector<CPoint> controlPoints;
static vector<CaliImage> caliImages; // 存储所有检校照片的数组
static ControlPointDlg *CpDlg;

SampleData::SampleData()
{
    CpDlg = new ControlPointDlg;
}

SampleData::~SampleData()
{
    currentImage.release();
}

void SampleData::showCpDlg()
{
    CpDlg->show();
}

bool SampleData::controlPointEmpty()
{
    if (controlPoints.empty())
        return true;
    else
        return false;
}
bool SampleData::caliImageEmpty()
{
    if (caliImages.empty())
        return true;
    else
        return false;
}

// 添加控制点，返回添加的位置
size_t SampleData::pushControlPoint(double x, double y, double z, int num)
{
    CPoint cp = {x, y, z, num};
    CpDlg->pushControlPoint(cp); // 存入对话框
    controlPoints.push_back(cp);
    return controlPoints.size()-1;
}
size_t SampleData::pushControlPoint(CPoint cp)
{
    controlPoints.push_back(cp);
    CpDlg->pushControlPoint(cp); // 存入对话框
    return controlPoints.size()-1;
}
// 添加影像，返回添加的位置
size_t SampleData::pushSampleData(CaliImage caliImage)
{
    caliImages.push_back(caliImage);
    return caliImages.size()-1;
}

// 获取标号为num的控制点
bool SampleData::getControlPoint(int num, CPoint *cp)
{
    if (controlPoints.empty())
    {
		// 控制点为空
		return false;
    }
    else
    {
        for (size_t i=0; i<controlPoints.size(); i++)
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
static bool getControlPoint(int num, CPoint *cp)
{
    if (controlPoints.empty())
    {
        // 控制点为空
        return false;
    }
    else
    {
        for (size_t i=0; i<controlPoints.size(); i++)
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
bool SampleData::getSampleData(size_t num, Mat *img)
{
    if (caliImages.empty())
    {
        return false;
    }
	else
	{
		// 超出图像范围
		if (num > caliImages.size()-1)
			return false;
		
        *img = caliImages[num].Image;
		return true;
	}
	
}

// 获取控制点数量
int SampleData::controlPointNumber()
{
    return int(controlPoints.size());
}
// 获取影像数量
int SampleData::caliImageNumber()
{
    return int(caliImages.size());
}

// 清空控制点
bool SampleData::clearControlPoint()
{
    controlPoints.clear();
    CpDlg->clearControlPoint();
    return true;
}
// 清空影像
bool SampleData::clearSampleData()
{
    caliImages.clear();
    return true;
}

static Rect select;
static bool select_flag;
static Mat rectImage;
static CaliImage caliImage;

// 绘制十字
static void drawCross(Mat img, Point center, int size=100, int thickness=1,  Scalar color=Scalar(0,0,255))
{
    // 绘制横线
    line(img,Point(center.x-size/2,center.y),Point(center.x+size/2,center.y),color,thickness);
    // 绘制竖线
    line(img,Point(center.x,center.y-size/2),Point(center.x,center.y+size/2),color,thickness);

}

// 在指定区域检测椭圆
static bool detectEllipse(Mat roiImg)
{
    Mat thrImg, showImg;

    showImg = roiImg.clone();
    cvtColor(showImg, thrImg, COLOR_BGR2GRAY);
    threshold(thrImg, thrImg, 100, 255, THRESH_BINARY);

    vector<vector<Point>> contours;
    findContours(thrImg, contours, RETR_LIST, CHAIN_APPROX_NONE);

    RotatedRect box;
    size_t i;
    for (i=0; i<contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area < 1e2 || 1e5 < area) continue;
        if (contours[i].size() < 100) continue;

        box = fitEllipse(contours[i]);

        ellipse(showImg, box, Scalar(0, 255, 0), 2);

        // 只画一个椭圆，一般第一个椭圆是ROI中最下方的那个
        break;
    }
    // 在框选图像上画十字
    drawCross(showImg, box.center, 30);

    // 显示框选图像
    namedWindow("Is it OK?", WINDOW_NORMAL);
    imshow("Is it OK?", showImg);

    if (QMessageBox::information(nullptr,
                             QObject::tr("提示"),
                             QString().sprintf("控制点中心坐标为 (%lf, %lf)", select.x + box.center.x, select.y + box.center.y),
                             QMessageBox::Yes,
                             QMessageBox::No)==QMessageBox::Yes)
    {
        // 在原图上画椭圆
        RotatedRect ellipseBox = box;
        ellipseBox.center.x += select.x;
        ellipseBox.center.y += select.y;

        ellipse(rectImage, ellipseBox, Scalar(0, 255, 0), 2);

        // 十字标
        drawCross(rectImage, ellipseBox.center, 30, 2);
        imshow("Detect Ellipse", rectImage);

        // 选择控制点
        SelectCpDlg *sCpDlg = new SelectCpDlg();
        sCpDlg->show();

        int num = sCpDlg->exec();
        if (num != -1)
        {
            CPoint cp;
            if (getControlPoint(num, &cp))
            {
                cp.x = double(ellipseBox.center.x);
                cp.y = double(ellipseBox.center.y);

                // 存入影像数组中
                caliImage.ControlPoints.push_back(cp);
            }
            else
            {
                QMessageBox::information(nullptr,
                                         QObject::tr("提示"),
                                         QObject::tr("控制点读取错误"));
            }

        }

        destroyWindow("Is it OK?");
    }
    else
    {
        destroyWindow("Is it OK?");
    }

    return true;
}

// 【静态成员变量】 鼠标事件监听
static void CPointMouseClick(int event, int x, int y, int flags, void *params)
{
    switch (event)
    {
    // 右键点击选取 ROI
    case EVENT_RBUTTONDOWN:
    {
        select.x = x;
        select.y = y;
        select_flag = true;
        break;
    }
    case EVENT_MOUSEMOVE:
    {
        if (select_flag)
        {
            Point p1, p2;
            currentImage.copyTo(rectImage);
            p1 = Point(select.x, select.y);
            p2 = Point(x, y);
            rectangle(rectImage, p1, p2, Scalar(0, 255, 0), 2);
            imshow("Detect Ellipse", rectImage);
        }
        break;
    }
    case EVENT_RBUTTONUP:
    {
        select_flag = false;
        //显示框出的ROI   
        Rect roi = Rect(Point(select.x, select.y), Point(x, y));
        if (roi.width && roi.height)
        {
            // 获取ROI影像
            Mat roiImg = currentImage(roi);
            // 在ROI影像上检测椭圆
            detectEllipse(roiImg);
        }
        break;
    }
    default:
    {
        break;
    }
    }
}

// 对指定像片进行检校
bool SampleData::calibration(size_t imageNum)
{
    Mat caliImage;
    if (getSampleData(imageNum, &caliImage))
    {
        // 显示照片
        namedWindow("Detect Ellipse", WINDOW_NORMAL);
        imshow("Detect Ellipse", caliImage);

        // 更新当前影像
        if (!currentImage.empty()) currentImage.release();
        currentImage = caliImage.clone();
        
        // 事件监听
        setMouseCallback("Detect Ellipse", CPointMouseClick);
        
        return true;
    }
    else
    {
        cout << "像片获取失败" << endl;
        return false;
    }
}
bool SampleData::calibration(QString imagePath)
{
    caliImage.ImagePath = imagePath;

    Mat caliImage = imread(imagePath.toLocal8Bit().data(), 1);
    if (!caliImage.empty() || caliImage.data)
    {
        namedWindow("Detect Ellipse", WINDOW_NORMAL);
        imshow("Detect Ellipse", caliImage);

        // 更新当前影像
        if (!currentImage.empty()) currentImage.release();
        currentImage = caliImage;

        // 事件监听
        setMouseCallback("Detect Ellipse", CPointMouseClick);

        return true;
    }
    else
    {
        cout << "像片打开失败" << endl;
        return false;
    }
}
