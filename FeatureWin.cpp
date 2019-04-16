#include "FeatureWin.h"
#include "ui_FeatureWin.h"

FeatureWin::FeatureWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FeatureWin)
{
    ui->setupUi(this);

    // 数据初始化
    imageDir = "F:/杉/文章/大三下/2. 数字摄影测量/imgs_for_digital_photogrammetry/";
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
    int leftHeight, leftWidth, rightHeight, rightWidth;
    leftHeight  = this->leftImage. rows;
    leftWidth   = this->leftImage. cols;
    rightHeight = this->rightImage.rows;
    rightWidth  = this->rightImage.cols;

    qDebug() << "图像尺寸：" << leftHeight << leftWidth << rightHeight << rightWidth;
    if (leftHeight != rightHeight || leftWidth != rightWidth)
    {
        showMessage("左右影像大小不一致");
        return;
    }

    int i, j;
    int destAreaSize = 3;
    int searchAreaSize = 49 + destAreaSize;
    for (i=0; i<leftHeight; i++)
    {
        for (j=0; j<leftWidth; j++)
        {

        }
    }
}
