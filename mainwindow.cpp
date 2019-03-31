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

#include "convert.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
                                                    tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
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
    showImage(inputImage);
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
        showImage(edgeImage, false);
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
        showImage(edgeImage, false);
    }
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
//    std::cout << viewWidth << ", " << viewHeight << std::endl;
//    std::cout << img.cols << ", " << img.rows << std::endl;
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
//            if (viewHeight < r * scale) scale = viewHeight / r; // 这步永远不会执行，因为已经无法满足 viewHeight < r，更不可能满足viewHeight < r * scale
        }

        disImage = disImage.scaled(int(c*scale), int(r*scale));
//        std::cout << scale << std::endl;
//        std::cout << c*scale << ", " << r*scale << std::endl;
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

    // 获取灰度图
    if (grayImage.empty())
        cvtColor(image, grayImage, COLOR_BGR2GRAY);
    // 使用高斯模糊
    int sigma = 3;
    int ksize = (sigma*5) | 1;
    if (blurImage.empty())
        GaussianBlur(grayImage, blurImage, Size(ksize, ksize), sigma, sigma);

    // 在灰度图上运行canny算子
    Mat edge;
    Canny(blurImage, edge, edgeThresh, edgeThresh*3, 3);
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

        showImage(edgeImage, false);
    }
}

void MainWindow::on_show_srcImage_triggered()
{
    if (!srcImage.empty())
    {
        showImage(srcImage, false);
        CannySlider->hide();
    }
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
    }
}

void MainWindow::on_fit_to_image_triggered()
{
    if (!edgeImage.empty()) resizeToImage(edgeImage);
    else if (!srcImage.empty()) resizeToImage(srcImage);
}

void MainWindow::releaseImages()
{
    if (!srcImage.empty()) srcImage.release();
    if (!edgeImage.empty()) edgeImage.release();
    if (!blurImage.empty()) blurImage.release();
    if (!grayImage.empty()) grayImage.release();
}

void MainWindow::on_edge_laplacian_triggered()
{
    if (!srcImage.empty())
    {
        if (edgeMethod == mLaplacian)
        {
            showImage(edgeImage, false);
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

            showImage(edgeImage, false);
        }
    }
}

void MainWindow::on_edge_log_triggered()
{
    if (!srcImage.empty())
    {
        if (edgeMethod == mLOG)
        {
            showImage(edgeImage, false);
        } else {
            edgeMethod = mLOG;

            Mat image = srcImage.clone();
            Mat laplace;

            // 获取灰度图
            if (grayImage.empty())
                cvtColor(image, grayImage, COLOR_BGR2GRAY);
            // 使用高斯模糊
            int sigma = 3;
            int ksize = (sigma*5) | 1;
            if (blurImage.empty())
                GaussianBlur(grayImage, blurImage, Size(ksize, ksize), sigma, sigma);


            Laplacian(blurImage, laplace, CV_16S, 5);
            convertScaleAbs(laplace, edgeImage, (sigma+1)*0.25);

            showImage(edgeImage, false);
        }
    }
}

void MainWindow::on_edge_sobel_triggered()
{
    if (!srcImage.empty())
    {
        if (edgeMethod == mSobel)
        {
            showImage(edgeImage, false);
        } else {
            edgeMethod = mSobel;

            Mat image = srcImage.clone();

            // 获取灰度图
            if (grayImage.empty())
                cvtColor(image, grayImage, COLOR_BGR2GRAY);
            // 使用高斯模糊
            int sigma = 3;
            int ksize = (sigma*5) | 1;
            if (blurImage.empty())
                GaussianBlur(grayImage, blurImage, Size(ksize, ksize), sigma, sigma);

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

            showImage(edgeImage, false);
        }
    }
}

void MainWindow::on_edge_roberts_triggered()
{
    cout << "roberts" << endl;
}

void MainWindow::on_edge_prewitt_triggered()
{
    cout << "prewitt" << endl;
}

void MainWindow::on_show_edge_triggered()
{
    if (!edgeImage.empty())
        showImage(edgeImage, false);
}

void MainWindow::on_show_blur_triggered()
{
    if (!blurImage.empty())
        showImage(blurImage, false);
}

void MainWindow::on_show_gray_triggered()
{
    if (!grayImage.empty())
        showImage(grayImage, false);
}
