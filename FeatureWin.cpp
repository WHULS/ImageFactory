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
    MoravecDlg *moravecDlg = new MoravecDlg();
    connect(moravecDlg, SIGNAL(ParamsChanged(int, int)), this, SLOT(moravecChanged(int, int)));
    moravecDlg->show();
    moravec(this->leftImage.clone(), 9, 9);
}

void FeatureWin::moravecChanged(int factorSize, int searchAreaSize)
{
    moravec(this->leftImage.clone(), factorSize, searchAreaSize);
}

void FeatureWin::moravec(Mat image, int factorSize, int searchAreaSize)
{
    int i, j, k, height, width;
    height = image.rows;
    width = image.cols;
    if (height <= 0 || width <= 0)
        return;
    qDebug() << tr("图像尺寸：") << height << width;

    qDebug() << image.channels();
    if (image.channels() == 3)
    {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }
    image.convertTo(image, CV_64F);


    // 1. 计算各个像元的兴趣值
    Mat interestValue(image.size(), image.type());
    for (i=factorSize/2; i<height-factorSize/2; i++)
    {
        for (j=factorSize/2; j<width-factorSize/2; j++)
        {
            // 每个点的兴趣值
            double v1, v2, v3, v4, minValue;
            v1=v2=v3=v4=0.0;
            for (k=-factorSize/2; k<factorSize/2; k++)
            {
                v1 += pow(image.at<double>(i, j+k) - image.at<double>(i, j+k+1)
                          , 2);
                v2 += pow(image.at<double>(i+k, j+k) - image.at<double>(i+k+1, j+k+1)
                          , 2);
                v3 += pow(image.at<double>(i+k, j) - image.at<double>(i+k+1, j)
                          , 2);
                v4 += pow(image.at<double>(i-k, j+k) - image.at<double>(i-k-1, j+k+1)
                          , 2);
            }
            minValue = min(v1, v2, v3, v4);
            interestValue.at<double>(i, j) = minValue;
        }
    }

    // 3. 计算阈值
    double threshold = mean(interestValue)[0];
    qDebug() << "阈值：" << threshold;

    // 4. 非极大值抑制
    Mat dilated, localMax;
    dilate(image, dilated, getStructuringElement(MORPH_RECT, Size(searchAreaSize, searchAreaSize)));
    compare(image, dilated, localMax, CMP_EQ);

    // 5. 获得角点
    Mat cornerMap;
    interestValue = interestValue > threshold;
    bitwise_and(localMax, interestValue, cornerMap);

    // 6. 绘制
    Mat shownImage;
    image.convertTo(shownImage, CV_8UC1);
    Mat_<uchar>::const_iterator it = cornerMap.begin<uchar>();
    Mat_<uchar>::const_iterator itd = cornerMap.end<uchar>();
    for (i=0; it!=itd; it++, i++)
    {
        if (*it)
            shownImage.at<uchar>(i/width, i%width) = 255;
    }
    showImage(shownImage);
//    // 计算各个像元的兴趣值

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
    ForstnerDlg *forstnerDlg = new ForstnerDlg();
    connect(forstnerDlg, SIGNAL(ParamsChanged(int, int, double, double)),
            this, SLOT(forstnerChanged(int,int,double,double)));
    forstnerDlg->show();
    forstner(this->leftImage.clone(), 9, 9);
}

void FeatureWin::forstnerChanged(int factorSize, int searchAreaSize, double Tq, double f)
{
    forstner(this->leftImage.clone(),factorSize, searchAreaSize, Tq, f);
}
/**
 * @param Tq 0.5~0.75
 * @param f  0.5~1.5
 */
void FeatureWin::forstner(Mat image,int factorSize, int searchAreaSize, double Tq, double f)
{
    int i, j, height, width;
    height = image.rows;
    width = image.cols;
    if (height <= 0 || width <= 0)
        return;
    qDebug() << tr("图像尺寸：") << height << width;

    qDebug() << image.channels();
    if (image.channels() == 3)
    {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }
    image.convertTo(image, CV_64F);


    // 1. 计算梯度
    Mat xKernel, yKernel, gu, gv, gu2, gv2, guv;
    xKernel = (Mat_<double>(1,3) << -1,0,1);
    yKernel = xKernel.t();

    filter2D(image, gu, CV_64F, xKernel);
    filter2D(image, gv, CV_64F, yKernel);

    // 2. 计算协方差阵
    gu2 = gu.mul(gu);
    gv2 = gv.mul(gv);
    guv = gu.mul(gv);
    // 高斯滤波
    int blurRadius = factorSize;
    double sigma = 1;
    Mat gaussianKernel = getGaussianKernel(blurRadius, sigma);
    filter2D(gu2, gu2, CV_64F, gaussianKernel);
    filter2D(gv2, gv2, CV_64F, gaussianKernel);
    filter2D(guv, guv, CV_64F, gaussianKernel);

    // 3. 兴趣值q和权w
    Mat interestValue(image.size(), image.type());
    Mat weightValue(image.size(), image.type());

    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            double det = gu2.at<double>(i, j)*gv2.at<double>(i, j) - guv.at<double>(i,j)*guv.at<double>(i,j);
            double tr = gu2.at<double>(i,j) + gv2.at<double>(i,j);
            interestValue.at<double>(i,j) = tr <=0 ? 0 :4*det/(tr*tr);
            weightValue.at<double>(i,j) = tr <=0 ? 0 :det/tr;
        }
    }

    // 4. 非极大值抑制(以w为依据)
    Mat dilated, localMax;
    dilate(weightValue, dilated, getStructuringElement(MORPH_RECT, Size(searchAreaSize, searchAreaSize)));
    compare(weightValue, dilated, localMax, CMP_EQ);

    // 5. 剔除小于阈值的点
    Mat cornerMap;
    interestValue = interestValue > Tq;
    double meanValue = mean(weightValue)[0];
    weightValue = weightValue > f*meanValue;
    bitwise_and(interestValue, meanValue, cornerMap);

    // 6. 得到结果并绘制
    bitwise_and(cornerMap, localMax, cornerMap);
    Mat shownImage;
    image.convertTo(shownImage, CV_8UC1);
    Mat_<uchar>::const_iterator it = cornerMap.begin<uchar>();
    Mat_<uchar>::const_iterator itd = cornerMap.end<uchar>();
    for (i=0; it!=itd; it++, i++)
    {
        if (*it)
            shownImage.at<uchar>(i/width, i%width) = 255;
    }
    showImage(shownImage);
}

void FeatureWin::on_harris_fetch_triggered()
{
    Mat leftCorner, rightCorner;
    harris(this->leftImage.clone(), leftCorner, 9, 1.5, 0.01);
    harris(this->rightImage.clone(), rightCorner, 9, 1.5, 0.01);

    showImage(leftCorner, rightCorner);

    this->leftCorner = leftCorner;
    this->rightCorner = rightCorner;
}

void FeatureWin::harris(Mat image, Mat &out, int blurRadius, double sigma, double qualityLevel)
{
    int i, j, height, width;
    height = image.rows;
    width = image.cols;
    if (height <= 0 || width <= 0)
        return;
    qDebug() << tr("图像尺寸：") << height << width;

    qDebug() << image.channels();
    if (image.channels() == 3)
    {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }


    // 转为浮点精度
    image.convertTo(image, CV_64F);

    // 卷积核
    Mat xKernel = (Mat_<double>(1,3) << -1, 0, 1);
    Mat yKernel = xKernel.t();

    // 差分图像
    Mat Ix, Iy;
    filter2D(image, Ix, CV_64F, xKernel);
    filter2D(image, Iy, CV_64F, yKernel);

    // 二次差分
    Mat Ix2, Iy2, Ixy;
    Ix2 = Ix.mul(Ix);
    Iy2 = Iy.mul(Iy);
    Ixy = Ix.mul(Iy);

    // 高斯滤波
    Mat gaussianKernel = getGaussianKernel(blurRadius, sigma);
    filter2D(Ix2, Ix2, CV_64F, gaussianKernel);
    filter2D(Iy2, Iy2, CV_64F, gaussianKernel);
    filter2D(Ixy, Ixy, CV_64F, gaussianKernel);

    // 兴趣值
    double k = 0.05; // 0.04~0.06
    Mat interestValue(image.size(), image.type());
    for (i=0; i<height; i++)
    {
        for (j=0; j<width; j++)
        {
            double det, tr;
            det = Ix2.at<double>(i,j) * Iy2.at<double>(i,j) - Ixy.at<double>(i,j) * Ixy.at<double>(i,j);
            tr = Ix2.at<double>(i,j) + Iy2.at<double>(i, j);
            interestValue.at<double>(i,j) = det - k*tr;
        }
    }

    // 确定阈值，提取角点
    Mat cornerMap;
    double maxValue;
    minMaxLoc(interestValue, NULL, &maxValue);
    Mat dilated, localMax;
    // 非极大值抑制(在膨胀的图像范围内剔除非极大值)
    dilate(interestValue, dilated, Mat());
    compare(interestValue, dilated, localMax, CMP_EQ);

    // 剔除小于阈值的值
    double threshold = qualityLevel * maxValue;
    cornerMap = interestValue > threshold;

    // 将大于阈值的点和局部极大值相关，得到角点
    bitwise_and(cornerMap, localMax, cornerMap);

    out = cornerMap;
}

Matrix FeatureWin::getGaussianFunction(int blurRadius, double sigma)
{
    if (blurRadius < 1)
    {
        qDebug() << "模糊半径过小";
        exit(-1);
    }
    int i, j;
    int size = blurRadius+1;
    Matrix weightMatrix(size, size);
    // 1. 计算权阵(上三角阵)
    for (i=0; i<size; i++)
    {
        for (j=i; j<size; j++)
        {
            weightMatrix[i][j] = exp(-(pow(i-blurRadius,2)+pow(j-blurRadius,2))/(2*sigma*sigma))/(2*PI*sigma*sigma);
        }
    }
    // 2. 获得高斯函数
    int row, col;
    row = 2*blurRadius+1;
    col = 2*blurRadius+1;
    Matrix gaussianFunction(row, col);
    for (i=0; i<size; i++)
    {
        for (j=i; j<size; j++)
        {
            gaussianFunction[i][j] = weightMatrix[i][j];
            gaussianFunction[j][i] = weightMatrix[i][j];
            gaussianFunction[row-i-1][j] = weightMatrix[i][j];
            gaussianFunction[j][row-i-1] = weightMatrix[i][j];
            gaussianFunction[i][col-j-1] = weightMatrix[i][j];
            gaussianFunction[col-j-1][i] = weightMatrix[i][j];
            gaussianFunction[row-i-1][col-j-1] = weightMatrix[i][j];
            gaussianFunction[col-j-1][row-i-1] = weightMatrix[i][j];
        }
    }
    // 归一化处理
    gaussianFunction = gaussianFunction / gaussianFunction.sum();

    return gaussianFunction;
}

void FeatureWin::drawCorner(Mat shownImage, Mat cornerMap, QString winName)
{
    // 绘制结果
    int i, width;
    width = shownImage.cols;
    shownImage.convertTo(shownImage, CV_8UC1);
    Mat_<uchar>::const_iterator it = cornerMap.begin<uchar>();
    Mat_<uchar>::const_iterator itd = cornerMap.end<uchar>();
    for (i=0; it!=itd; it++, i++)
    {
        if (*it)
        {
            shownImage.at<uchar>(i/width, i%width) = 255;
        }
    }

    imshow(winName.toLocal8Bit().data(), shownImage);
}

void FeatureWin::on_correlation_index_triggered()
{
    if (leftCorner.empty() || rightCorner.empty())
    {
        showMessage("请先进行角点提取");
        return;
    }

    Mat lImg, rImg;
    leftImage.convertTo(lImg, CV_64F);
    rightImage.convertTo(rImg, CV_64F);

    Mat_<uchar>::const_iterator lit, rit, litd, ritd;
    lit  = leftCorner. begin<uchar>();
    litd = leftCorner. end<uchar>();

    int nsize = 7; // 邻域大小

    int i, j, width_left, width_right, height_left, height_right, row_left, col_left, row_right, col_right;
    height_left  = leftCorner .rows;
    width_left   = leftCorner .cols;
    height_right = rightCorner.rows;
    width_right  = rightCorner.cols;

    int count=0;
    for (i=0; lit!=litd; lit++, i++)
    {
        if (!*lit) continue;

        row_left = i/width_left;
        col_left = i%width_left;

        // 以角点为中心，提取左边窗口

        // 在右边以所有角点为中心，提取窗口

        // 右边逐一与左边对比，得到相关系数最大的窗口中的那个角点作为匹配点

        // 放弃边缘点
        if (row_left-nsize<0 || row_left+nsize>=height_left ||
                col_left-nsize<0 || col_left+nsize>=width_left)
        {
            continue;
        }
        // 左窗口
        Mat leftWin = lImg(Range(row_left-nsize, row_left+nsize),
                           Range(col_left-nsize, col_left+nsize));

        // 相关系数
        double correlationValue = -9999.9;

        rit  = rightCorner.begin<uchar>();
        ritd = rightCorner.end<uchar>();

        for (j=0; rit!=ritd; rit++, j++)
        {
            if (!*rit) continue;

            row_right = j/width_right;
            col_right = j%width_right;

            if (row_right-nsize<0 || row_right+nsize>=height_right ||
                    col_right-nsize<0 || col_right+nsize>=width_right)
            {
                continue;
            }
            // 右窗口
            Mat rightWin = rImg(Range(row_right-nsize, row_right+nsize),
                               Range(col_right-nsize, col_right+nsize));

            double coVal = correlation(leftWin, rightWin);
            correlationValue = coVal > correlationValue ? coVal : correlationValue;
        }
        if (correlationValue>0)
            count++;
    }
    qDebug() << count;
}

double FeatureWin::correlation(Mat win1, Mat win2)
{
    int h1,w1,h2,w2;
    h1 = win1.rows;
    w1 = win1.cols;
    h2 = win2.rows;
    w2 = win2.cols;
    if (h1!=h2 || w1!=w2)
    {
        qDebug() << "左右窗口大小不一" << h1 << w1 << h2 << w2;
        exit(-1);
    }
    if (win1.type()!=CV_64F)
    {
        win1.convertTo(win1, CV_64F);
    }
    if (win2.type()!=CV_64F)
    {
        win2.convertTo(win2, CV_64F);
    }

    double sum12, sum1, sum2, sum_1, sum_2;
    sum12 = double(h1*w1)*mean(win1.mul(win2))[0];
    sum1 = double(h1*w1)*mean(win1)[0];
    sum2 = double(h1*w1)*mean(win2)[0];
    sum_1 = double(h1*w1)*mean(win1.mul(win1))[0];
    sum_2 = double(h1*w1)*mean(win2.mul(win2))[0];

    double correlationValue;
    correlationValue = (sum12 - sum1*sum2/double(h1*w1))/
            sqrt((sum_1-sum1*sum1/double(h1*w1))*((sum_2-sum2*sum2/double(h1*w1))));
    return correlationValue;
}
