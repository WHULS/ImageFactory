#include "CalibrationWin.h"
#include "ui_CalibrationWin.h"

CalibrationWin::CalibrationWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CalibrationWin)
{
    ui->setupUi(this);
    imageDir = "F:/杉/文章/大三下/3. 卫星摄影测量/第四次作业/";
}

CalibrationWin::~CalibrationWin()
{
    delete ui;
}

void CalibrationWin::on_open_image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("打开一张图像"),
                                                    imageDir,
                                                    tr("Image (*.png *.jpg *.jpeg)"));
    if (fileName.isEmpty()) return;

    image = imread(fileName.toLocal8Bit().data(), 1);
    if (image.empty())
    {
        showMessage("打开图像失败");
        return;
    }
    showImage(image);
}

void CalibrationWin::on_calibration_triggered()
{
    int cols = 9;
    int rows = 6;
    float distance = 30;	//间距30mm

    Size patternSize(rows, cols);
    vector<Point2f> corners;
    vector<vector<Point2f>> cornersVect;
    vector<Point3f> worldPoints;
    vector<vector<Point3f>> worldPointsVect;

    for (int i=0;i<cols;i++)
    {
        for (int j=0;j<rows;j++)
        {
            worldPoints.push_back(Point3f(i*distance,j*distance,0));
        }
    }

    Mat sampleImage = image.clone();
    cvtColor(sampleImage, sampleImage, COLOR_BGR2GRAY);

    bool find = findChessboardCorners(sampleImage, patternSize, corners);
    drawChessboardCorners(sampleImage, patternSize, corners, find);

    showImage(sampleImage, "Sample Image");

    Mat cameraMatirx, distCoeffs;
    vector<Mat> rvecs,tvecs,rvecs2,tvecs2;
    if (find)
    {
        cornersVect.push_back(corners);
        worldPointsVect.push_back(worldPoints);
        calibrateCamera(worldPointsVect,cornersVect,sampleImage.size(),cameraMatirx,distCoeffs,rvecs,tvecs);

        print(cameraMatirx);
        print(distCoeffs);
        print(rvecs);
        print(tvecs);
        QString caliInfoStr = QString().sprintf("等效焦距(fx,fy)=(%.3lf,%.3lf)\n像主点(cx,cy)=(%.1lf,%.1lf)",
                                                cameraMatirx.at<double>(0, 0),
                                                cameraMatirx.at<double>(1, 1),
                                                cameraMatirx.at<double>(0, 2),
                                                cameraMatirx.at<double>(1, 2));
        caliInfoStr += QString("\n畸变系数(k1,k2,p1,p2[,k3[,k4,k5,k6[,s1,s2,s3,s4[,tx,ty]]]]) :\n");
        for (int i=0; i<distCoeffs.cols; i++)
        {
            caliInfoStr += QString().sprintf("%.8lf,", distCoeffs.at<double>(0, i));
        }
        caliInfoStr += QString().sprintf("外方位元素：\nφ=%.6lf\nω=%.6lf\nκ=%.6lf\nXs=%.3lf\nYs=%.3lf\nZs=%.3lf",
                rvecs[0].at<double>(0,0), rvecs[0].at<double>(1,0), rvecs[0].at<double>(2,0),
                tvecs[0].at<double>(0,0), tvecs[0].at<double>(1,0), tvecs[0].at<double>(2,0));
        showMessage(caliInfoStr, "检校信息");
    }

}

int CalibrationWin::showMessage(QString str, QString header)
{
    return QMessageBox::information(this,
                             header,
                             str);
}

void CalibrationWin::showImage(Mat img, QString str)
{
    namedWindow(str.toLocal8Bit().data());
    imshow(str.toLocal8Bit().data(), img);
}

void CalibrationWin::print(Mat mat)
{
    int i, j;
    int height, width;
    height = mat.rows;
    width = mat.cols;
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            cout << mat.at<double>(i, j) << " ";
        }
        cout << endl;
    }
    cout << "-----" << endl;
}

void CalibrationWin::print(vector<Mat> matVec)
{
    size_t i, length;
    int j, k, height, width;
    length = matVec.size();

    for (i=0; i<length; i++)
    {
        height = matVec[i].rows;
        width = matVec[i].cols;
        for (j=0; j<height; j++)
        {
            for (k=0; k<width; k++)
            {
                cout << matVec[i].at<double>(j, k) << " ";
            }
            cout << endl;
        }
        cout << "-----" << endl;
    }
}
