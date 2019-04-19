#include "FeatureWin.h"
#include "ui_FeatureWin.h"

FeatureWin::FeatureWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FeatureWin)
{
    ui->setupUi(this);

    // 数据初始化
    imageDir = "E:/杉/文章/大三下/2. 数字摄影测量/imgs_for_digital_photogrammetry/";
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
                                                tr("Raw File (*.raw)"));
    if (lImagePath.isEmpty()) return;
    QFile lFile(lImagePath);
    if (!lFile.open(QFile::ReadOnly))
    {
        showMessage(tr("左影像打开失败"));
        return;
    }

    // 打开右影像文件
    rImagePath = QFileDialog::getOpenFileName(this,
                                                  tr("打开右影像"),
                                                  this->imageDir,
                                                  tr("Raw File (*.raw)"));
    if (rImagePath.isEmpty()) return;
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
    Mat image = this->leftImage.clone();
    int height, width;
    height = image.rows;
    width = image.cols;

    qDebug() << tr("图像尺寸：") << height << width;

    int i, j, k;
    int factorSize = 5;
    double threshold = 0.0;

    // 计算各个像元的兴趣值
    Mat interestValueMat = image.zeros(height, width, CV_64F);
    qDebug() << interestValueMat.rows << interestValueMat.cols;

    for (i=factorSize/2; i<height-factorSize/2; i++)
    {
        for (j=factorSize/2; j<width-factorSize/2; j++)
        {
            // 每个点的兴趣值
            double v1, v2, v3, v4, minValue;
            v1=v2=v3=v4=0.0;
            for (k=-factorSize/2; k<factorSize/2; k++)
            {
                v1 += pow(image.at<uchar>(i, j+k) - image.at<uchar>(i, j+k+1), 2);
                v2 += pow(image.at<uchar>(i+k, j+k) - image.at<uchar>(i+k+1, j+k+1), 2);
                v3 += pow(image.at<uchar>(i+k, j) - image.at<uchar>(i+k+1, j), 2);
                v4 += pow(image.at<uchar>(i-k, j+k) - image.at<uchar>(i-k-1, j+k+1), 2);
            }
            minValue = min(v1, v2, v3, v4);
            interestValueMat.at<double>(i, j) = minValue;

            threshold += minValue;
        }
    }

    // 选取候选点
    threshold /= (height-factorSize/2) * (width - factorSize/2); // 阈值

    for (i=factorSize/2; i<height-factorSize/2; i++)
    {
        for (j=factorSize/2; j<width-factorSize/2; j++)
        {
            if (interestValueMat.at<double>(i,j) - max(interestValueMat(Range(i-factorSize, i+factorSize), Range(j-factorSize, j+factorSize))) < 1e-5
                    && interestValueMat.at<double>(i,j) > threshold)
            {
                circle(image, Point(i,j), 3, Scalar(0,0,255), 3);
            }
        }
    }
    imshow("a", image);
}

double FeatureWin::min(double v1, double v2, double v3, double v4)
{
    double out = v1;
    if (v2 < out) out = v2;
    if (v3 < out) out = v3;
    if (v4 < out) out = v4;
    return out;
}

double FeatureWin::max(Mat mat)
{
    double max = mat.at<double>(0,0);
    int i, j, height, width;
    height = mat.rows;
    width = mat.cols;
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            if (mat.at<double>(i,j)>max)
            {
                max = mat.at<double>(i,j);
            }
        }
    }
    return max;
}
