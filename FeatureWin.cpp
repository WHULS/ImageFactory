#include "FeatureWin.h"
#include "ui_FeatureWin.h"

FeatureWin::FeatureWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FeatureWin)
{
    ui->setupUi(this);

    // 数据初始化
    imageDir = "E:/杉/文章/大三下/2. 数字摄影测量/实习/imgs_for_digital_photogrammetry/";
}

FeatureWin::~FeatureWin()
{
    delete ui;
}

void FeatureWin::on_open_image_triggered()
{
    QString lImagePath, rImagePath;

    // 打开左影像文件
    lImagePath= QFileDialog::getOpenFileName(this,
                                             tr("打开左影像"),
                                             this->imageDir,
                                             tr("Raw File (*.raw);;Image File (*.bmp *.jpg *.jpeg *.png)"));
    if (lImagePath.isEmpty()) return;
    this->imageDir = lImagePath.section("/", 0, -2);
    // 打开右影像文件
    rImagePath = QFileDialog::getOpenFileName(this,
                                              tr("打开右影像"),
                                              this->imageDir,
                                              tr("Raw File (*.raw);;Image File (*.bmp *.jpg *.jpeg *.png)"));
    if (rImagePath.isEmpty()) return;
    this->imageDir = rImagePath.section("/", 0, -2);

    qDebug() << "文件格式: " << rImagePath.section(".", -1, -1);

    if (rImagePath.section(".", -1, -1) == "raw")
    {
        QFile lFile(lImagePath);
        if (!lFile.open(QFile::ReadOnly))
        {
            showMessage(tr("左影像打开失败"));
            return;
        }
        QFile rFile(rImagePath);
        if (!rFile.open(QFile::ReadOnly))
        {
            showMessage(tr("右影像打开失败"));
            return;
        }
        // 数据数组
        QByteArray lArr, rArr;
        lArr = lFile.readAll();
        rArr = rFile.readAll();

        // 将数据输入到QImage中(宽1240，高1210)
        QImage lImage((uchar*)lArr.data(), 1240, 1210, QImage::Format_Grayscale8);
        QImage rImage((uchar*)rArr.data(), 1240, 1210, QImage::Format_Grayscale8);

        lImage.bits();

        // copy是硬拷贝，将数据和格式一起拷贝
        this->qLeftImage = lImage.copy();
        this->qRightImage = rImage.copy();

        this->leftImage = QImage2cvMat(qLeftImage, true);
        this->rightImage = QImage2cvMat(qRightImage, true);
    }
    else
    {
        this->leftImage = imread(lImagePath.toLocal8Bit().data(), 1);
        this->rightImage = imread(rImagePath.toLocal8Bit().data(), 1);
        if (this->leftImage.empty())
        {
            showMessage(tr("左影像打开失败"));
            return;
        }
        if (this->rightImage.empty())
        {
            showMessage(tr("右影像打开失败"));
            return;
        }
        this->qLeftImage = cvMat2QImage(this->leftImage);
        this->qRightImage = cvMat2QImage(this->rightImage);
    }

    showImage(leftImage, rightImage);
}

int FeatureWin::showMessage(QString str)
{
    return QMessageBox::information(this,
                                    tr("提示"),
                                    str);
}

void FeatureWin::showImage(Mat l, Mat r)
{
    QImage lImage, rImage;
    lImage = cvMat2QImage(l);
    rImage = cvMat2QImage(r);
    showImage(lImage, rImage);
}

void FeatureWin::showImage(QImage l, QImage r)
{
    QPixmap lPixmap, rPixmap;
    lPixmap = QPixmap::fromImage(l);
    rPixmap = QPixmap::fromImage(r);

    int width = ui->Image_Left->geometry().width();
    int height = ui->Image_Left->geometry().height();

    ui->Image_Left->setPixmap(lPixmap.scaled(width, height, Qt::KeepAspectRatio));
    ui->Image_Right->setPixmap(rPixmap.scaled(width, height, Qt::KeepAspectRatio));
}
void FeatureWin::showImage(Mat image)
{
    QImage qImage;
    qImage = cvMat2QImage(image);
    QPixmap imagePixmap;
    imagePixmap = QPixmap::fromImage(qImage);

    int width = ui->Image_Left->geometry().width();
    int height = ui->Image_Left->geometry().height();

    ui->Image_Left->setPixmap(imagePixmap.scaled(width, height, Qt::KeepAspectRatio));
}

void FeatureWin::wheelEvent(QWheelEvent *event)
{
    event->accept();

    if (QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        QPoint numPixels = event->pixelDelta();
        QPoint numDegrees = event->angleDelta() / 8;

        int step = 0;
        if (!numPixels.isNull())
        {
            step = numPixels.y();
        }
        else if (!numDegrees.isNull())
        {
            QPoint numSteps = numDegrees / 15;
            step = numSteps.y();
        }

//        qDebug() << step;

        // TODO: 图像放大
        if (zoomScale >= 100 && step > 0)
            return;
        if (zoomScale < 1 && step < 0)
            return;

        zoomScale += 2*step;

        int height = int(double(ui->Image_Left->geometry().height() * zoomScale)/100.0);
        int width = int(double(ui->Image_Left->geometry().width() * zoomScale)/100.0);

        // 构建缩放图像
        QPixmap lPixmap, rPixmap;
        lPixmap = QPixmap::fromImage(qLeftImage);
        rPixmap = QPixmap::fromImage(qRightImage);

//        qDebug() << height << ", " << width;

        if (height <=0 || width <=0)
            return;

        ui->Image_Left->setPixmap(lPixmap.scaled(width, height, Qt::KeepAspectRatio));
        ui->Image_Right->setPixmap(rPixmap.scaled(width, height, Qt::KeepAspectRatio));
    }
}

void FeatureWin::on_moravec_fetch_triggered()
{
    PointFeatureDlg *featureDlg = new PointFeatureDlg();
    connect(featureDlg, SIGNAL(ParamsChanged(int, int)), this, SLOT(moravecChanged(int, int)));
    featureDlg->show();
    moravec(this->leftImage.clone());
}

void FeatureWin::moravecChanged(int factorSize, int searchAreaSize)
{
    moravec(this->leftImage.clone(), factorSize, searchAreaSize);
}

void FeatureWin::moravec(Mat image, int factorSize, int searchAreaSize)
{
    qDebug() << image.channels();
    if (image.channels() == 3)
    {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }

    int height, width;
    height = image.rows;
    width = image.cols;

    if (height <= 0 || width <= 0)
        return;

    qDebug() << tr("图像尺寸：") << height << width;

    int i, j, k;
    double threshold = 0.0;

    // 计算各个像元的兴趣值
    Matrix interestValue(height, width);

    for (i=factorSize/2; i<height-factorSize/2; i++)
    {
        for (j=factorSize/2; j<width-factorSize/2; j++)
        {
            // 每个点的兴趣值
            double v1, v2, v3, v4, minValue;
            v1=v2=v3=v4=0.0;
            for (k=-factorSize/2; k<factorSize/2; k++)
            {
                v1 += pow(image.at<uchar>(i, j+k) - image.at<uchar>(i, j+k+1)
                          , 2);
                v2 += pow(image.at<uchar>(i+k, j+k) - image.at<uchar>(i+k+1, j+k+1)
                          , 2);
                v3 += pow(image.at<uchar>(i+k, j) - image.at<uchar>(i+k+1, j)
                          , 2);
                v4 += pow(image.at<uchar>(i-k, j+k) - image.at<uchar>(i-k-1, j+k+1)
                          , 2);
            }
            minValue = min(v1, v2, v3, v4);
            interestValue[i][j] = minValue;

            threshold += minValue;
        }
    }
//    interestValue.print();

    // 选取候选点
    threshold /= height*width; // 阈值
    qDebug() << "阈值：" << threshold;

    Mat showImage = image.clone();
    for (i=searchAreaSize/2; i<height-searchAreaSize/2; i++)
    {
        for (j=searchAreaSize/2; j<width-searchAreaSize/2; j++)
        {
            // 剔除小于阈值的点
            if (interestValue[i][j]<threshold) continue;

            // 非极大值抑制
            double maxValue = interestValue(i-searchAreaSize/2, i+searchAreaSize/2,j-searchAreaSize/2, j+searchAreaSize/2)
                    .max();
            if (interestValue[i][j] == maxValue)
                showImage.at<uchar>(i, j) = 255;
        }
    }
//    this->showImage(showImage);
    imshow("Moravec", showImage);
}

double FeatureWin::min(double v1, double v2, double v3, double v4)
{
    double out = v1;
    if (v2 < out) out = v2;
    if (v3 < out) out = v3;
    if (v4 < out) out = v4;
    return out;
}

void FeatureWin::on_forstner_fetch_triggered()
{
    Mat image = this->leftImage.clone();
    qDebug() << image.channels();
    if (image.channels() == 3)
    {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }

    int i, j, k, height, width;
    height = image.rows;
    width = image.cols;
    int factorSize = 5;
    int searchAreaSize = 5;

    Matrix interestValue(height, width);
    Matrix weightValue(height, width);
    Mat shownImage = image.clone();
    for (i=factorSize/2; i<height-factorSize/2; i++)
    {
        for (j=factorSize/2; j<width-factorSize/2; j++)
        {
            Matrix N(2,2);
            double gu, gv;
            for (k=-factorSize/2; k<factorSize/2; k++)
            {
                // 求每个像素的robert梯度
                gu = image.at<uchar>(i+k+1, j+k+1) - image.at<uchar>(i+k, j+k);
                gv = image.at<uchar>(i-k, j+k+1) - image.at<uchar>(i-k-1, j+k);
                // 灰度协方差矩阵
                N[0][0] += gu * gu;
                N[0][1] += gu * gv;
                N[1][0] += gv * gu;
                N[1][1] += gv * gv;
            }
            // 计算兴趣值和权
            double q, w;
            q = 4*N.det() / pow(N.tr(), 2);
            w = N.det() / N.tr();
            interestValue[i][j] = q;
            weightValue[i][j] = w;
        }
    }

    // 经验阈值
    double Tq, Tw, f;
    Tq = 0.5; // 0.5~0.75
    f = 0.75; // 0.5~1.5
    Tw = f * weightValue.mean();
    for (i=searchAreaSize/2; i<height-searchAreaSize/2; i++)
    {
        for (j=searchAreaSize/2; j<width-searchAreaSize/2; j++)
        {
            // 剔除小于阈值的点
            if (interestValue[i][j] < Tq || weightValue[i][j]<Tw) continue;

            // 非极大值抑制
            double maxValue = weightValue(i-searchAreaSize/2, i+searchAreaSize/2, j-searchAreaSize/2, j+searchAreaSize/2)
                    .max();
            if (maxValue-weightValue[i][j] < 1e-5)
                shownImage.at<uchar>(i,j)=255;
        }
    }
    imshow("Forstner", shownImage);
}

void FeatureWin::on_harris_fetch_triggered()
{

}
