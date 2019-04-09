#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <QMessageBox> // 消息框
#include <QFileDialog> // 文件框
#include <QGraphicsScene> // 图形场景
#include <QMouseEvent> // 鼠标事件
#include <QKeyEvent> // 键盘事件
#include <QWheelEvent> // 鼠标滚动事件
#include <QGuiApplication>
#include <QScreen>

/* UI */
#include <QGridLayout>
#include <houghcircledlg.h>

#include "convert.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化样本图像类
    sampleImages = new SampleImage();

    // 屏幕大小
    QScreen *pSc = QGuiApplication::primaryScreen();
    availableWidth = pSc->availableGeometry().width();
    availableHeight = pSc->availableGeometry().height();

//    this->setGeometry(0, 0, availableWidth, availableHeight); // 设置软件大小
//    this->showFullScreen(); // 全屏（沉浸式）
//    this->showMaximized();  // 最大化
//    std::cout << ui->toolBar->geometry().height() << std::endl;
//    std::cout << ui->menuBar->geometry().height() << std::endl;

    // 初始化显示场景
    scene = new QGraphicsScene;
    ImageView = new MyGraphicsView;
    ImageView->setScene(scene);
    ImageView->hide();

    // 隐藏滚动条
    ImageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ImageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 设置拖动模式
    ImageView->setDragMode(QGraphicsView::ScrollHandDrag);

    // 初始化拖动条
    CannySlider = new MySlider(Qt::Orientation::Vertical);
    CannySlider->hide();
    CannySlider->setObjectName("CannySlider");
    // 连接拖动条的数值变化信号与响应槽
    connect(CannySlider, SIGNAL(valueChanged(int)), this, SLOT(On_CannySlider_valueChanged(int)));

    // 新建栅格布局管理器
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(ImageView, 0, 0, 10, 9);
    layout->addWidget(CannySlider, 0, 10, 10, 1);
    // 将布局添加到中心控件上
    ui->centralWidget->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_image_open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    this->imagePath,
                                                    tr("Image File(*.png *.jpg *.jpeg *.bmp *.raw)"));
    this->imagePath = fileName.section("/", 0, -2); // 保存图像位置，作为下次打开的根目录

    if (fileName.isEmpty()) return;

    // 清空已有图像
    releaseImages();

    Mat inputImage = imread(fileName.toLocal8Bit().data(), 1);

    if (!inputImage.empty() && inputImage.data) {
        srcImage = inputImage;
        this->CannySlider->hide();
    } else {
        QMessageBox::warning(this,
                             tr("打开图像失败！"),
                             tr("图像为空"),
                             QMessageBox::Yes);
        return;
    }

    // 显示图像
    showImage(inputImage, "Src Image");
}

void MainWindow::on_clear_image_triggered()
{
    if (!srcImage.empty())
    {
        // 隐藏场景
        scene->clear();
        ImageView->hide();

        // 隐藏 Canny 拖动条
        CannySlider->hide();

        // 清空图像
        releaseImages();
    }
}

void MainWindow::on_edge_canny_triggered()
{
    if (srcImage.empty()) {
        showMessageBox(tr("请先打开一张图像"));
    } else if (edgeMethod == mCanny){
        CannySlider->show();
        showImage(edgeImage, "Canny Edge");
    } else {
        edgeMethod = mCanny;
        // 对话框
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Icon::Information,
                                              tr("运算中"),
                                              tr("运算中"),
                                              QMessageBox::NoButton,
                                              this);
        msgBox->show();
        CannySlider->show();

        // 边缘检测
        edgeImage = edgeDetectCanny(srcImage, CannySlider->value()).clone();

        msgBox->hide();

        // 显示
        showImage(edgeImage, "Canny Edge");
    }
}

void MainWindow::showImage(Mat img, const char winName[])
{
    namedWindow(winName, WINDOW_NORMAL);
    imshow(winName, img);
}

void MainWindow::showImage(Mat img, bool isResize)
{
    // 转换数据（包含在 convert.h 中）
    QImage disImage = cvMat2QImage(img);

    // 调整窗口大小
    if (isResize) {
        resizeToImage(img);
    }

    // 调整图像大小
    int viewWidth = this->geometry().width() - 23;
    int viewHeight = this->geometry().height() - ui->menuBar->height() - ui->toolBar->height() - 23;
    if (viewWidth < img.cols || viewHeight < img.rows) {
        // 自动调整尺寸比较大的图片
        int c = img.cols;
        int r = img.rows;

        double scale = 1.0; // 缩放比例
        if (viewHeight < r) {
            scale = double(viewHeight) / double(r);
            if (viewWidth < c * scale) scale = viewWidth / c;
        } else if (viewWidth < c) {
            scale = double(viewWidth) / double(c);
        }

        disImage = disImage.scaled(int(c*scale), int(r*scale));
    }

    scene->clear();
    scene->addPixmap(QPixmap::fromImage(disImage));
    ImageView->show();
}

Mat MainWindow::edgeDetectCanny(Mat img, int edgeThresh)
{
    Mat edgeImage;

    if (img.empty()) {
        return edgeImage;
    } else {
        edgeImage.create(img.size(), img.type());
    }
    
    // 深拷贝
    Mat image = img.clone();

    Mat grayImage, blurImage;
    // 获取灰度图
    cvtColor(image, grayImage, COLOR_BGR2GRAY);
    // 模糊
    blur(grayImage, blurImage, Size(3,3));

    this->blurImage = blurImage.clone();

    // 在灰度图上运行canny算子
    Mat edge;
    Canny(grayImage, edge, edgeThresh, edgeThresh*3, 3);
    edgeImage = Scalar::all(0);

    // 获取边缘图像(上色) edgeImage
    image.copyTo(edgeImage, edge);

    return  edgeImage;
}

void MainWindow::On_CannySlider_valueChanged(int threshod)
{
    if (!srcImage.empty())
    {
        edgeImage = edgeDetectCanny(srcImage, threshod).clone();

        showImage(edgeImage, "Canny Edge");
    }
}

void MainWindow::on_show_srcImage_triggered()
{
    if (!srcImage.empty())
    {
        showImage(srcImage, "Src Image");
        CannySlider->hide();
    } else showMessageBox(tr("请先打开一张图像"));
}

void MainWindow::showMessageBox(QString msg)
{

    QMessageBox::warning(this,
                         tr("提示"),
                         msg);
}

void MainWindow::resizeToImage(Mat img)
{
    if (!img.empty()) {
        int appWidth = img.cols + 23;
        int appHeight = img.rows + ui->menuBar->height() + ui->toolBar->height() + 23;
        // 如果大于屏幕，则直接全屏显示
        if (appWidth >= availableWidth || appHeight >= availableHeight) {
            this->setGeometry(0, 0, availableWidth, availableHeight);
            this->showMaximized();
        }
        // 否则适应图片
        else {
            int leftTopX = (availableWidth - appWidth) / 2;
            int lettTopY = (availableHeight - appHeight) / 2;

            if (this->isMaximized()) this->showNormal();
            this->setGeometry(leftTopX, lettTopY, appWidth, appHeight);
        }
    } else showMessageBox(tr("请先打开一张图像"));
}

void MainWindow::on_fit_to_image_triggered()
{
    if (!edgeImage.empty()) resizeToImage(edgeImage);
    else if (!srcImage.empty()) resizeToImage(srcImage);
    else showMessageBox(tr("请先打开一张图像"));
}

void MainWindow::releaseImages()
{
    if (!srcImage.empty()) srcImage.release();
    if (!edgeImage.empty()) edgeImage.release();
    if (!blurImage.empty()) blurImage.release();
    if (!grayImage.empty()) grayImage.release();
    edgeMethod = -1;
}

void MainWindow::on_edge_laplacian_triggered()
{
    if (!srcImage.empty())
    {
        if (edgeMethod == mLaplacian)
        {
            showImage(edgeImage, "Laplacian Edge");
        } else {
            edgeMethod = mLaplacian;

            Mat image = srcImage.clone();
            Mat laplace;

            // 获取灰度图
            if (grayImage.empty())
                cvtColor(image, grayImage, COLOR_BGR2GRAY);
            int sigma = 3;

            Laplacian(grayImage, laplace, CV_16S, 5);
            convertScaleAbs(laplace, edgeImage, (sigma+1)*0.25);

            showImage(edgeImage, "Laplacian Edge");
        }
    } else showMessageBox(tr("请先打开一张图像"));
}

void MainWindow::on_edge_log_triggered()
{
    if (!srcImage.empty())
    {
        if (edgeMethod == mLOG)
        {
            showImage(edgeImage, "LOG Edge");
        } else {
            edgeMethod = mLOG;

            Mat image = srcImage.clone();
            Mat laplace;

            Mat grayImage, blurImage;
            int sigma = 3;
            int ksize = (sigma*5) | 1;
            // 获取灰度图
            cvtColor(image, grayImage, COLOR_BGR2GRAY);
            // 使用高斯模糊
            GaussianBlur(grayImage, blurImage, Size(ksize, ksize), sigma, sigma);

            this->blurImage = blurImage.clone();


            Laplacian(blurImage, laplace, CV_16S, 5);
            convertScaleAbs(laplace, edgeImage, (sigma+1)*0.25);

            showImage(edgeImage, "LOG Edge");
        }
    } else showMessageBox(tr("请先打开一张图像"));
}

void MainWindow::on_edge_sobel_triggered()
{
    if (!srcImage.empty())
    {
        if (edgeMethod == mSobel)
        {
            showImage(edgeImage, "Sobel Edge");
        } else {
            edgeMethod = mSobel;

            Mat image = srcImage.clone();

            Mat grayImage, blurImage;
            int sigma = 3;
            int ksize = (sigma*5) | 1;
            // 获取灰度图
            cvtColor(image, grayImage, COLOR_BGR2GRAY);
            // 使用高斯模糊
            GaussianBlur(grayImage, blurImage, Size(ksize, ksize), sigma, sigma);

            this->blurImage = blurImage.clone();

            Mat grad_x, grad_y;
            Mat abs_grad_x, abs_grad_y;
            // 一堆参数
            int ddepth = CV_16S;
            ksize = 3;
            int scale = 1;
            int delta = 0;

            Sobel(blurImage, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
            Sobel(blurImage, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);

            convertScaleAbs(grad_x, abs_grad_x);
            convertScaleAbs(grad_y, abs_grad_y);

            addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, edgeImage);

            equalizeHist(edgeImage, edgeImage);

            showImage(edgeImage, "Sobel Edge");
        }
    } else showMessageBox(tr("请先打开一张图像"));
}

void MainWindow::on_edge_roberts_triggered()
{
    if (!srcImage.empty())
    {
        if (edgeMethod == mRoberts)
        {
            showImage(edgeImage, "Roberts Edge");
        }
        else
        {
            edgeMethod = mRoberts;

            Mat image = srcImage.clone();
            Mat gray, blur;
            cvtColor(image, gray, COLOR_BGR2GRAY);
            int sigma = 3;
            int ksize = (sigma*5) | 1;
            GaussianBlur(gray, blur, Size(ksize, ksize), sigma, sigma);

            int height=gray.rows, width=gray.cols;
            Mat edge(height, width, CV_8UC1);

            for (int i=0; i<height-1; i++)
            {
                for (int j=0; j<width-1; j++)
                {
                    edge.at<uchar>(i,j) = uchar(
                                abs( blur.at<uchar>(i,j)*-1 + blur.at<uchar>(i+1,j+1) )/2
                                +
                                abs( blur.at<uchar>(i,j+1)*-1 + blur.at<uchar>(i+1,j) )/2
                            );
                }
            }
            equalizeHist(edge, edge);
            edgeImage = edge.clone();
            showImage(edgeImage, "Roberts Edge");
        }
    } else showMessageBox(tr("请先打开一张图像"));
}

void MainWindow::on_edge_prewitt_triggered()
{
    if (!srcImage.empty())
    {
        if (edgeMethod == mPrewitt)
        {
            showImage(edgeImage, "Prewitt Edge");
        }
        else
        {
            edgeMethod = mPrewitt;

            Mat image = srcImage.clone();
            Mat gray, blur;
            cvtColor(image, gray, COLOR_BGR2GRAY);
            int sigma = 3;
            int ksize = (sigma*5) | 1;
            GaussianBlur(gray, blur, Size(ksize, ksize), sigma, sigma);

            int height=gray.rows, width=gray.cols;
            Mat edge(height, width, CV_8UC1);

            for (int i=1; i<height-1; i++)
            {
                for (int j=1; j<width-1; j++)
                {
                    edge.at<uchar>(i,j) =
                            uchar(
                                abs(
                                    (blur.at<uchar>(i-1, j+1) + blur.at<uchar>(i, j+1) + blur.at<uchar>(i+1, j+1))*-1 +
                                    blur.at<uchar>(i-1, j-1) + blur.at<uchar>(i, j-1) + blur.at<uchar>(i+1, j-1)
                                ) /2
                                +
                                abs(
                                    (blur.at<uchar>(i-1, j-1) + blur.at<uchar>(i-1, j) + blur.at<uchar>(i-1, j+1))*-1 +
                                    blur.at<uchar>(i+1, j-1) + blur.at<uchar>(i+1, j) + blur.at<uchar>(i+1, j+1)
                                ) /2
                            );
                }
            }
            equalizeHist(edge, edge);
            edgeImage = edge.clone();
            showImage(edgeImage, "Prewitt Edge");
        }
    } else showMessageBox(tr("请先打开一张图像"));
}

void MainWindow::on_show_edge_triggered()
{
    if (!edgeImage.empty())
        showImage(edgeImage, "Current Edge");
     else
        showMessageBox(tr("请先进行边缘提取"));
}

void MainWindow::on_show_blur_triggered()
{
    if (blurImage.empty())
    {
        if (!srcImage.empty())
        {
            Mat image = srcImage.clone();
            int sigma = 3;
            int ksize = (sigma*5) | 1;
            // 获取灰度图
            if (grayImage.empty())
                cvtColor(image, grayImage, COLOR_BGR2GRAY);
            // 使用高斯模糊
            GaussianBlur(grayImage, blurImage, Size(ksize, ksize), sigma, sigma);
        }
        else {
            showMessageBox(tr("请先打开一张图像"));
            return;
        }
    }
    showImage(blurImage, "Blur");
}

void MainWindow::on_show_gray_triggered()
{
    if (grayImage.empty())
    {
        if (!srcImage.empty())
        {
            Mat image = srcImage.clone();
            cvtColor(image, grayImage, COLOR_BGR2GRAY);
        }
        else {
            showMessageBox(tr("请先打开一张图像"));
            return;
        }

    }
    showImage(grayImage, "Gray");
}

/**
 * @brief MainWindow::difference_of_gaussian
 * @param image
 * @param sigma
 * @param ksize
 * @param k 高斯模糊之间的倍数
 * @return DOG 边缘图像
 */
Mat MainWindow::difference_of_gaussian(Mat image, int sigma, int ksize, int k)
{
    Mat gray;
    Mat blur1, blur2;

    cvtColor(image, gray, COLOR_BGR2GRAY);

    GaussianBlur(gray, blur1, Size(ksize, ksize), sigma, sigma);
    GaussianBlur(gray, blur2, Size(ksize, ksize), sigma*k, sigma*k);

    Mat edge;
    addWeighted(blur2, 1, blur1, -1, 0, edge);

    // 直方图均衡化
    equalizeHist(edge, edge);

    return edge;
}

void MainWindow::on_edge_dog_triggered()
{
    if (!srcImage.empty())
    {
        if (edgeMethod == mDOG)
        {
            showImage(edgeImage, "DOG Edge");
        }
        else
        {
            edgeMethod = mDOG;
            int sigma = 3;
            edgeImage = difference_of_gaussian(srcImage.clone(), sigma, (sigma*5 | 1));
            showImage(edgeImage, "DOG Edge");
        }
    } else showMessageBox(tr("请先打开一张图像"));
}

void MainWindow::on_calibration_triggered()
{
    /*if(!srcImage.empty())
    {
        // Hough 变换检测圆
//        Mat image = srcImage.clone();
//        HoughCircleDetect(image, HOUGH_GRADIENT, 1, image.rows/8, 100, 30, 1, 30);

//        // 创建对话框
//        HoughCircleDlg *hcDlg = new HoughCircleDlg(this);
//        hcDlg->show();

//        // 连接信号和槽
//        connect(hcDlg, SIGNAL(ParamsChanged(double, double, double, double, int, int)),
//                this, SLOT(On_HoughCircle_valueChanged(double, double, double, double, int, int)));

        Mat image = srcImage.clone();
        // 用 DOG 算子检测边缘（弃用：轮廓过于明显，产生双环）
        Mat edge = difference_of_gaussian(image);
        // 腐蚀，缩小轮廓
        erode(edge, edge, getStructuringElement(MORPH_RECT, Size(6,6)));
        showImage(edge);

        vector<vector<Point>> contours;
        findContours(edge, contours, RETR_LIST, CHAIN_APPROX_NONE);


        // 绘制轮廓线
//        for (size_t i=0; i<contours.size(); i++)
//        {
//            // 计算每个 contour 的面积
//            double area = contourArea(contours[i]);

//            // 忽略太小或太大的区域
//            if (area < 1e2 || 1e5 < area) continue;
//            cout << contours[i].size() << endl;

//            // 绘制 contour
//            drawContours(image, contours, static_cast<int>(i), Scalar(0,0,255), 2);
//        }

//        showImage(image);
        RotatedRect box;
        int count = 0;
        for (size_t i=0; i<contours.size(); i++)
        {
            double area = contourArea(contours[i]);
            if (area < 1e2 || 1e5 < area) continue;
            if (contours[i].size() < 100) continue;

            box = fitEllipse(contours[i]);

            double boxRatio = double(box.size.height)/double(box.size.width);
            if (boxRatio > 5) continue;

            ellipse(image, box, Scalar(0, 255, 0), 2);
            putText(image, to_string(count), box.center, FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 255), 2);

            count++;
        }

        imshow("Hello?", image);
    }*/

    // 获取文件路径
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    this->imagePath,
                                                    tr("Image File(*.png *.jpg *.jpeg *.bmp *.raw)"));
    this->imagePath = fileName.section("/", 0, -2); // 保存图像位置，作为下次打开的根目录
    if (fileName.isEmpty()) return;
}

/** Hough 检测图像中的圆并显示(原图输入)
 * @brief MainWindow::HoughCircleDetect
 * @param image
 * @param method
 * @param dp
 * @param minDist
 * @param param1
 * @param param2
 * @param minRadius
 * @param maxRadius
 */
void MainWindow::HoughCircleDetect(Mat image, int method, double dp, double minDist, double param1, double param2, int minRadius, int maxRadius)
{
    Mat gray;

    cvtColor(image, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, gray, Size(9,9), 3, 3);

    vector<Vec3f> circles;

    HoughCircles(gray, circles, method, dp, minDist, param1, param2, minRadius, maxRadius);

    for (size_t i=0; i<circles.size(); i++) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);

        // draw the circle center
        circle(image, center, 1, Scalar(0, 100, 100), 3, LINE_AA);

        // draw the circle outline
        circle(image, center, radius, Scalar(0, 0, 255), 3, LINE_AA);
    }

    showImage(image, "Hough");
}

// 匹配 HoughDlg 的插槽
void MainWindow::On_HoughCircle_valueChanged(double dp, double minDist, double param1, double param2, int minRadius, int maxRadius)
{
    if (!srcImage.empty())
    {
        Mat image = srcImage.clone();
        HoughCircleDetect(image, HOUGH_GRADIENT, dp, minDist, param1, param2, minRadius, maxRadius);
    }
}

void MainWindow::on_read_control_point_triggered()
{
    if (sampleImages->controlPointEmpty())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Control Point"),
                                                        this->cpPath,
                                                        tr("控制点文件(*.txt)"));
        this->cpPath = fileName.section("/", 0, -2);

        FILE *fp = fopen(fileName.toLocal8Bit().data(), "r");
        if (fp)
        {
            int beginPos, pointNum;
            fscanf(fp, "%d %d\n", &beginPos, &pointNum); // 获取控制点文件头信息

            QMessageBox::information(this,
                                     tr("头信息"),
                                     QString().sprintf("起始点号：%d, 控制点数目：%d\n", beginPos, pointNum));

            while (!feof(fp))
            {
                CPoint cPoint;
                int t;
                fscanf(fp, "%d %lf %lf %lf %d\n %d\n", &cPoint.num, &cPoint.x, &cPoint.y, &cPoint.z, &t, &t);

//                cout << cPoint.num << ": " << cPoint.x << ", " << cPoint.y << ", " << cPoint.z << endl;
                sampleImages->pushControlPoint(cPoint);
            }

            // 关闭文件
            fclose(fp);

            ui->read_control_point->setText(tr("清空控制点(&O)"));
        }

    }
    else
    {
        if (QMessageBox::information(this,
                                 tr("提示"),
                                 tr("是否清空控制点信息？"),
                                 QMessageBox::Yes,
                                 QMessageBox::No) == QMessageBox::Yes)
        {
            sampleImages->clearControlPoint();
            ui->read_control_point->setText(tr("读取控制点文件(&O)"));
        }
    }
}

void MainWindow::on_point_Moravec_triggered()
{

}
