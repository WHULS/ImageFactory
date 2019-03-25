#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <QMessageBox> // 消息框
#include <QFileDialog> // 文件框
#include <QGraphicsScene> // 图形场景
#include <QMouseEvent> // 鼠标事件
#include <QKeyEvent> // 键盘事件
#include <QWheelEvent> // 鼠标滚动事件

/* UI */
#include <QGridLayout>

#include "convert.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    Mat inputImage = imread(fileName.toLocal8Bit().data(), 1);

    if (!inputImage.empty() || !inputImage.data) {
        srcImage = inputImage;
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

    }
}

void MainWindow::on_edge_canny_triggered()
{
    if (srcImage.empty()) {
        QMessageBox::warning(this,
                             tr("提示"),
                             tr("请先打开一张图像"));
    } else {
        // 对话框
        QMessageBox *infoBox = new QMessageBox;
        infoBox->setIcon(QMessageBox::Information);
        infoBox->setWindowTitle("提示");
        infoBox->setText("运算中");
        infoBox->show();

        CannySlider->show();

        Mat result = edgeDetectCanny(srcImage, CannySlider->value());

        infoBox->setText("运算结束");
        infoBox->close();

        // 显示
        showImage(result);
    }
}

void MainWindow::showImage(Mat img)
{

    // 转换数据（包含在 convert.h 中）
    QImage disImage = cvMat2QImage(img);

    // 获取显示区尺寸
    QSize winSize = ImageView->size();

    // 用 QGraphicsScene 管理图像数据
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(disImage.scaled(winSize)));

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
    std::cout << threshod << std::endl;
    if (!srcImage.empty())
    {
        Mat edge = edgeDetectCanny(srcImage, threshod);

        showImage(edge);
    }
}

void MainWindow::on_show_srcImage_triggered()
{
    if (!srcImage.empty())
        showImage(srcImage);
}
