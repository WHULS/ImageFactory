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
                                                    ".",
                                                    tr("Image File(*.png *.jpg *.jpeg *.bmp)"));

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
        srcImage.release();
        if (!cedge.empty()) cedge.release();

    }
}

void MainWindow::on_edge_canny_triggered()
{
    if (srcImage.empty()) {
        showMessageBox(tr("请先打开一张图像"));
    } else if (!cedge.empty()){
        CannySlider->show();
        showImage(cedge, false);
    } else {
        // 对话框
        QMessageBox *infoBox = new QMessageBox;
        infoBox->setIcon(QMessageBox::Information);
        infoBox->setWindowTitle("提示");
        infoBox->setText("运算中");
        infoBox->show();

        CannySlider->show();

        cedge = edgeDetectCanny(srcImage, CannySlider->value()).clone();

        infoBox->setText("运算结束");
        infoBox->close();

        // 显示
        showImage(cedge, false);
    }
}

void MainWindow::showImage(Mat img, bool isResize)
{
    // 调整图像大小
    if (!isResize) { // 在满屏幕情况下，若图像尺寸过大，需要调整
        int viewWidth = this->geometry().width() - 23;
        int viewHeight = this->geometry().height() - ui->menuBar->height() - ui->toolBar->height() - 23;

        if (viewWidth < img.cols || viewHeight < img.rows) {
            // TODO: 自动调整尺寸比较大的图片
        }
    }

    // 转换数据（包含在 convert.h 中）
    QImage disImage = cvMat2QImage(img);

    // 获取显示区尺寸
//    QSize winSize = ImageView->size();

    // 用 QGraphicsScene 管理图像数据
//    scene->clear();
//    scene->addPixmap(QPixmap::fromImage(disImage.scaled(winSize)));

//    this->ImageView->setGeometry(0, 0, img.cols, img.rows);
    if (isResize) {
        resizeToImage(img);
    }

    scene->clear();
    scene->addPixmap(QPixmap::fromImage(disImage));

    ImageView->show();
}

Mat MainWindow::edgeDetectCanny(Mat img, int edgeThresh)
{
    Mat cedge;

    if (img.empty()) {
        return cedge;
    } else {
        cedge.create(img.size(), img.type());
    }
    
    // 深拷贝
    Mat image = img.clone();

    // 获取灰度图
    Mat gray, blurImage;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    blur(gray, blurImage, Size(3, 3));

    // 在灰度图上运行canny算子
    Mat edge;
    Canny(blurImage, edge, edgeThresh, edgeThresh*3, 3);
    cedge = Scalar::all(0);

    // 获取边缘图像 cedge
    image.copyTo(cedge, edge);

    return  cedge;
}

void MainWindow::On_CannySlider_valueChanged(int threshod)
{
    if (!srcImage.empty())
    {
        cedge = edgeDetectCanny(srcImage, threshod).clone();

        showImage(cedge, false);
    }
}

void MainWindow::on_show_srcImage_triggered()
{
    if (srcImage.empty()) showMessageBox(tr("请先打开一张图像"));
    else{
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
    if (!cedge.empty()) resizeToImage(cedge);
    else if (!srcImage.empty()) resizeToImage(srcImage);
}

void MainWindow::releaseImages()
{
    if (!srcImage.empty()) srcImage.release();
    if (!cedge.empty()) cedge.release();
}
