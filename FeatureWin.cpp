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
                                             tr("Image File (*.raw *.bmp *.jpg *.jpeg *.png)"));
    if (lImagePath.isEmpty()) return;
    this->imageDir = lImagePath.section("/", 0, -2);
    // 打开右影像文件
    rImagePath = QFileDialog::getOpenFileName(this,
                                              tr("打开右影像"),
                                              this->imageDir,
                                              tr("Image File (*.raw *.bmp *.jpg *.jpeg *.png)"));
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
void FeatureWin::merge(Mat left, Mat right, Mat &merged)
{
    if (left.type() != right.type())
    {
        qDebug() << "合并图像类型不一致";
        exit(-1);
    }
    int leftHeight, leftWidth, rightHeight, rightWidth;
    leftHeight = left.rows;
    leftWidth = left.cols;
    rightHeight = right.rows;
    rightWidth = right.cols;

    Mat mImage(MAX(leftHeight, rightHeight), leftWidth + rightWidth, left.type());
    Mat mergeLeft, mergeRight;
    mergeLeft = mImage(Rect(0, 0, left.cols, left.rows));
    mergeRight = mImage(Rect(left.cols, 0, right.cols, right.rows));
    left.copyTo(mergeLeft);
    right.copyTo(mergeRight);

    merged = mImage.clone();
}
void FeatureWin::showMergeImage(Mat left, Mat right)
{
    Mat mImage;
    merge(left, right, mImage);
    namedWindow("merge", WINDOW_NORMAL);
    imshow("merge", mImage);
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

    moravecChanged(9, 9);
}

/**
 * @brief FeatureWin::moravecChanged 响应moravec对话框，执行moravec算法
 * @param factorSize      目标区大小
 * @param searchAreaSize  搜索区大小
 */
void FeatureWin::moravecChanged(int factorSize, int searchAreaSize)
{
    Mat leftCorner, rightCorner;
    moravec(leftImage.clone(), leftCorner, factorSize, searchAreaSize);
    moravec(rightImage.clone(), rightCorner, factorSize, searchAreaSize);

    Mat left, right;
    left = leftImage.clone();
    right = rightImage.clone();

    // 绘制角点
    drawCorner(left, leftCorner);
    drawCorner(right, rightCorner);

    // 显示图像
    showMergeImage(left, right);
    showImage(left, right);

    this->leftCornerImage = left;
    this->rightCornerImage = right;

    this->leftCorner = leftCorner;
    this->rightCorner = rightCorner;
}

/**
 * @brief FeatureWin::moravec
 * @param image
 * @param out
 * @param factorSize
 * @param searchAreaSize
 */
void FeatureWin::moravec(Mat image, Mat &out, int factorSize, int searchAreaSize)
{
    int height, width;
    height = image.rows;
    width = image.cols;
    if (height <= 0 || width <= 0)
        return;
    cout << "图像尺寸：" << height << ", " << width;

    cout << image.channels();
    if (image.channels() == 3)
    {
        cvtColor(image, image, COLOR_BGR2GRAY);
    }
    image.convertTo(image, CV_64F);


    // 1. 计算梯度
    Mat xxKernel, yyKernel, xyKernel, yxKernel;
    xxKernel = (Mat_<double>(1, 2) << -1, 1);
    yyKernel = xxKernel.t();
    xyKernel = (Mat_<double>(2, 2) << -1, 0,
        0, 1);
    yxKernel = (Mat_<double>(2, 2) << 0, -1,
        1, 0);
    // 梯度差
    Mat lxx, lyy, lxy, lyx;
    filter2D(image, lxx, CV_64F, xxKernel);
    filter2D(image, lyy, CV_64F, yyKernel);
    filter2D(image, lxy, CV_64F, xyKernel);
    filter2D(image, lyx, CV_64F, yxKernel);
    // 梯度差平方
    lxx = lxx.mul(lxx);
    lyy = lyy.mul(lyy);
    lxy = lxy.mul(lxy);
    lyx = lyx.mul(lyx);

    // 2. 计算各个像元的兴趣值
    Mat xxValue, yyValue, xyValue, yxValue;
    Mat xxValueKernel, yyValueKernel, xyValueKernel, yxValueKernel;
    xxValueKernel = (Mat_<double>(1, factorSize, 1.0));
    yyValueKernel = xxValueKernel.t();
    xyValueKernel = (Mat_<double>(factorSize, factorSize, 0.0));
    yxValueKernel = (Mat_<double>(factorSize, factorSize, 0.0));
    for (int i = 0; i<factorSize; i++)
    {
        xyValueKernel.at<double>(i, i) = 1.0;
        yxValueKernel.at<double>(i, factorSize - 1 - i) = 1.0;
    }
    // 四个方向的方差和
    filter2D(lxx, xxValue, CV_64F, xxValueKernel);
    filter2D(lyy, yyValue, CV_64F, yyValueKernel);
    filter2D(lxy, xyValue, CV_64F, xyValueKernel);
    filter2D(lyx, yxValue, CV_64F, yxValueKernel);
    // 计算最小值
    Mat bitcomp, interestValue;
    bitcomp = xxValue < yyValue;
    bitcomp.convertTo(bitcomp, CV_64F, 1.0 / 255.0);
    interestValue = xxValue.mul(bitcomp) + yyValue.mul(1.0 - bitcomp); // xx和yy中每一位上的较小数

    bitcomp = interestValue < xyValue;
    bitcomp.convertTo(bitcomp, CV_64F, 1.0 / 255.0);
    interestValue = interestValue.mul(bitcomp) + xyValue.mul(1.0 - bitcomp); // xx,yy,xy的最小

    bitcomp = interestValue < yxValue;
    bitcomp.convertTo(bitcomp, CV_64F, 1.0 / 255.0);
    interestValue = interestValue.mul(bitcomp) + yxValue.mul(1.0 - bitcomp); // xx,yy,xy,yx的最小

    // 3. 计算阈值
    double threshold = mean(interestValue)[0];
    cout << "阈值：" << threshold;

    // 4. 非极大值抑制
    Mat dilated, localMax;
    dilate(image, dilated, getStructuringElement(MORPH_RECT, Size(searchAreaSize, searchAreaSize)));
    compare(image, dilated, localMax, CMP_EQ);

    // 5. 获得角点
    Mat cornerMap;
    interestValue = interestValue > threshold;
    bitwise_and(localMax, interestValue, cornerMap);

    out = cornerMap;
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

    forstnerChanged(9, 9, 0.5, 0.75);
}

/**
 * @brief FeatureWin::forstnerChanged 响应Forstner对话框，执行forstner算法
 * @param factorSize      目标区大小
 * @param searchAreaSize  搜索区大小
 * @param Tq              阈值1 0.5~0.75
 * @param f               阈值2 0.5~1.5
 */
void FeatureWin::forstnerChanged(int factorSize, int searchAreaSize, double Tq, double f)
{
    Mat leftCorner, rightCorner;
    forstner(this->leftImage.clone(), leftCorner, factorSize, searchAreaSize, Tq, f);
    forstner(this->rightImage.clone(), rightCorner, factorSize, searchAreaSize, Tq, f);
    this->leftCorner = leftCorner;
    this->rightCorner = rightCorner;

    Mat left, right;
    left = leftImage.clone();
    right = rightImage.clone();

    // 绘制角点
    drawCorner(left, leftCorner);
    drawCorner(right, rightCorner);

    // 显示图像
    showMergeImage(left, right);
    showImage(left, right);

    this->leftCornerImage = left;
    this->rightCornerImage = right;

    this->leftCorner = leftCorner;
    this->rightCorner = rightCorner;
}
/**
 * @param image           输入图像
 * @param out             输出角点图
 * @param factorSize      目标区大小
 * @param searchAreaSize  搜索区大小
 * @param Tq              阈值1 0.5~0.75
 * @param f               阈值2 0.5~1.5
 */
void FeatureWin::forstner(Mat image, Mat &out, int factorSize, int searchAreaSize, double Tq, double f)
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
//    Mat shownImage;
//    image.convertTo(shownImage, CV_8UC1);
//    Mat_<uchar>::const_iterator it = cornerMap.begin<uchar>();
//    Mat_<uchar>::const_iterator itd = cornerMap.end<uchar>();
//    for (i=0; it!=itd; it++, i++)
//    {
//        if (*it)
//            shownImage.at<uchar>(i/width, i%width) = 255;
//    }
//    showImage(shownImage);
    out = cornerMap.clone();
}

void FeatureWin::on_harris_fetch_triggered()
{
    Mat leftCorner, rightCorner;
    harris(this->leftImage.clone(), leftCorner, 9, 1.5, 0.01);
    harris(this->rightImage.clone(), rightCorner, 9, 1.5, 0.01);

    Mat left, right;
    left = leftImage.clone();
    right = rightImage.clone();

    // 绘制角点
    drawCorner(left, leftCorner);
    drawCorner(right, rightCorner);

    // 显示图像
    showMergeImage(left, right);
    showImage(left, right);

    this->leftCornerImage = left;
    this->rightCornerImage = right;

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
    minMaxLoc(interestValue, nullptr, &maxValue);
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

void FeatureWin::on_correlation_index_triggered()
{
    if (leftCorner.empty() || rightCorner.empty())
    {
        showMessage("请先进行角点提取");
        return;
    }

    // 初始化表格
    initTable();
    QStringList strList;
    strList << "No" << "left X" << "left Y" << "right X" << "right Y" << "value";
    createTable(strList);

    Mat lImg, rImg;
    leftImage.convertTo(lImg, CV_64F);
    rightImage.convertTo(rImg, CV_64F);

    Mat_<uchar>::const_iterator lit, rit, litd, ritd;
    lit  = leftCorner. begin<uchar>();
    litd = leftCorner. end<uchar>();

    int nsize = 7;          // 计算窗口大小
    int ssize = nsize * 20; // 搜索区大小

    int leftWidth, rightWidth, leftHeight, rightHeight;
    leftHeight  = leftCorner .rows;
    leftWidth   = leftCorner .cols;
    rightHeight = rightCorner.rows;
    rightWidth  = rightCorner.cols;


    int i, j, row_left, col_left, row_right, col_right;
    for (i=0; lit!=litd; lit++, i++)
    {
        if (!*lit) continue;

        row_left = i/leftWidth;
        col_left = i%leftWidth;

        // 放弃边缘点
        if (row_left-nsize<0 || row_left+nsize>=leftHeight ||
                col_left-nsize<0 || col_left+nsize>=leftWidth)
        {
            continue;
        }

        // 左窗口
        Mat leftWin = lImg(Range(row_left-nsize, row_left+nsize),
                           Range(col_left-nsize, col_left+nsize));

        // 搜索区
        int r1, r2, c1, c2;
        r1 = row_left-ssize < 0 ? 0 : row_left-ssize;
        r2 = row_left+ssize > leftHeight ? leftHeight : row_left+ssize;
        c1 = col_left-ssize < 0 ? 0 : col_left-ssize;
        c2 = col_left+ssize > leftWidth ? leftWidth : col_left+ssize;

        Mat rightSearchArea = rightCorner(Range(r1, r2), Range(c1, c2));

        int searchHeight, searchWidth;
        searchHeight = rightSearchArea.rows;
        searchWidth = rightSearchArea.cols;

        // 相关系数
        double correlationValue = -9999.9;

        rit  = rightSearchArea.begin<uchar>();
        ritd = rightSearchArea.end<uchar>();


        MatchPoint mPt;
        mPt.lpt = Point(col_left, row_left);
        for (j=0; rit!=ritd; rit++, j++)
        {
            if (!*rit) continue;

            row_right = j/searchWidth;
            col_right = j%searchWidth;

            if ((row_right-nsize<0 || row_right+nsize>=rightHeight) || // 匹配窗口越界
                (col_right-nsize<0 || col_right+nsize>=rightWidth))
            {
                continue;
            }
            // 右窗口
            Mat rightWin = rImg(Range(row_right-nsize, row_right+nsize),
                               Range(col_right-nsize, col_right+nsize));

            double coVal = correlation(leftWin, rightWin);
            if (coVal > correlationValue)
            {
                correlationValue = coVal;
                mPt.rpt = Point(col_right + c1, row_right + r1);
            }
        }
        mPt.cVal = correlationValue;
        if (correlationValue > 0.5)
            matchPoints.push_back(mPt);
    }

    // 在表格中显示匹配信息
    presentMatchInfo(matchPoints);
    // 绘制所有匹配线
    showMatchImage(leftImage.clone(), rightImage.clone(), matchPoints);
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

void FeatureWin::initTable()
{
    // 获取数据视图
    QTableView *pointInfoView;
    pointInfoView = ui->Information_Table;
    pointInfoView->verticalHeader()->hide();
    pointInfoView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置为不可修改
    pointInfoView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 数据模型
    this->pointInfoModel = new QStandardItemModel();
    pointInfoView->setModel(this->pointInfoModel);
}

void FeatureWin::createTable(QStringList strList)
{
    int i;
    for (i=0; i<strList.size(); i++)
    {
        QString str = static_cast<QString>(strList.at(i));
        this->pointInfoModel->setHorizontalHeaderItem(i, new QStandardItem(str));
    }
}

void FeatureWin::presentMatchInfo(vector<MatchPoint> matchPoints)
{
    size_t i;
    // 填充数据
    for (i=0; i<matchPoints.size(); i++)
    {
        this->pointInfoModel->setItem(i,0, new QStandardItem(QString().sprintf("%d", i)));

        // left x
        this->pointInfoModel->setItem(i,1, new QStandardItem(QString().sprintf("%d", matchPoints[i].lpt.x)));
        // left y
        this->pointInfoModel->setItem(i,2, new QStandardItem(QString().sprintf("%d", matchPoints[i].lpt.y)));
        // right x
        this->pointInfoModel->setItem(i,3, new QStandardItem(QString().sprintf("%d", matchPoints[i].rpt.x)));
        // right y
        this->pointInfoModel->setItem(i,4, new QStandardItem(QString().sprintf("%d", matchPoints[i].rpt.y)));
        // correlation value
        this->pointInfoModel->setItem(i,5, new QStandardItem(QString().sprintf("%lf", matchPoints[i].cVal)));
    }
    // 设置点击事件
    connect(ui->Information_Table, SIGNAL(clicked(QModelIndex)), this, SLOT(showPoint(QModelIndex)));
    // 设置排序方式
    this->pointInfoModel->sort(5, Qt::DescendingOrder);
}

void FeatureWin::showPoint(QModelIndex idx)
{
    int row = idx.row();

    // 匹配的点
    Point lpt, rpt;
    lpt = matchPoints[row].lpt;
    rpt = matchPoints[row].rpt;

    // 合成图像
    Mat left, right;
    left = leftCornerImage.clone();
    right = rightCornerImage.clone();

    cout << lpt << endl;
    cout << rpt << endl;

    if (left.channels() == 3)
        circle(left, lpt, 30, Scalar(0, 0, 255), 2);
    else
        circle(left, lpt, 5, Scalar(255), 2);
    if (right.channels() == 3)
        circle(right, rpt, 30, Scalar(0, 0, 255), 2);
    else
        circle(right, rpt, 5, Scalar(255), 2);

    showImage(left, right);
    showMatchImage(left, right, matchPoints[row]);
}

void FeatureWin::drawCorner(Mat &input, Mat corner, int thickness, int radius, Scalar color)
{
    if (corner.channels()==3)
    {
        qDebug() << "角点图像不是灰度图";
        return;
    }

    int i, height, width;
    height = corner.rows;
    width = corner.cols;
    if (height != input.rows || width != input.cols)
    {
        qDebug() << "角点和原图尺寸不匹配";
        return;
    }

    Mat_<uchar>::const_iterator it = corner.begin<uchar>();
    Mat_<uchar>::const_iterator itd = corner.end<uchar>();
    if (input.channels()==3)
    {
        for (i=0; it!=itd; it++, i++)
        {
            if (*it)
                circle(input, Point(i%width, i/width), radius, color, thickness);
        }
    }
    else
    {
        qDebug() << "Tip: 灰度图无法显示色彩信息";

        for (i=0; it!=itd; it++, i++)
        {
            if (*it)
                input.at<uchar>(i/width, i%width) = 255;
        }
    }
}

void FeatureWin::showMatchImage(Mat left, Mat right, vector<MatchPoint> matchPoints)
{
    int leftHeight, leftWidth, rightHeight, rightWidth;
    if (this->leftCornerImage.empty() || this->rightCornerImage.empty())
    {
        qDebug() << "角点影像为空";
        exit(-1);
    }
    if (left.channels() != right.channels())
    {
        qDebug() << "两幅影像通道数不一致";
        exit(-1);
    }

    leftHeight = left.rows;
    leftWidth = left.cols;
    rightHeight = right.rows;
    rightWidth = right.cols;

    Mat merge;
    this->merge(left, right, merge);

    if (left.channels() == 3)
    {
        for (size_t i=0; i<matchPoints.size(); i++)
        {
            Point lpt, rpt;
            lpt = matchPoints[i].lpt;
            rpt = matchPoints[i].rpt + Point(leftWidth, 0);
            circle(merge, lpt, 15, Scalar(0, 0, 255), 1);
            circle(merge, rpt, 15, Scalar(0, 0, 255), 1);
            line(merge, lpt, rpt, Scalar(255, 0, 0));
        }
    }
    else
    {
        for (size_t i=0; i<matchPoints.size(); i++)
        {
            Point lpt, rpt;
            lpt = matchPoints[i].lpt;
            rpt = matchPoints[i].rpt + Point(leftWidth, 0);
            circle(merge, lpt, 15, Scalar(255));
            circle(merge, rpt, 15, Scalar(255));
            line(merge, lpt, rpt, Scalar(255));
        }
    }
    namedWindow("Match Image", WINDOW_NORMAL);
    imshow("Match Image", merge);
}

void FeatureWin::showMatchImage(Mat left, Mat right, MatchPoint point)
{
    int leftHeight, leftWidth, rightHeight, rightWidth;
    if (this->leftCornerImage.empty() || this->rightCornerImage.empty())
    {
        qDebug() << "角点影像为空";
        exit(-1);
    }
    if (left.channels() != right.channels())
    {
        qDebug() << "两幅影像通道数不一致";
        exit(-1);
    }

    leftHeight = left.rows;
    leftWidth = left.cols;
    rightHeight = right.rows;
    rightWidth = right.cols;

    Mat merge;
    this->merge(left, right, merge);

    if (left.channels() == 3)
    {
        Point lpt, rpt;
        lpt = point.lpt;
        rpt = point.rpt + Point(leftWidth, 0);
        circle(merge, lpt, 15, Scalar(0, 0, 255), 1);
        circle(merge, rpt, 15, Scalar(0, 0, 255), 1);
        line(merge, lpt, rpt, Scalar(255, 0, 0));
    }
    else
    {
        Point lpt, rpt;
        lpt = point.lpt;
        rpt = point.rpt + Point(leftWidth, 0);
        circle(merge, lpt, 15, Scalar(255));
        circle(merge, rpt, 15, Scalar(255));
        line(merge, lpt, rpt, Scalar(255));
    }

    namedWindow("Match Image", WINDOW_NORMAL);
    imshow("Match Image", merge);
}

void FeatureWin::on_least_square_triggered()
{
    if (leftImage.empty() || rightImage.empty())
    {
        qDebug() << "影像为空";
        return;
    }
    if (leftCorner.empty() || rightCorner.empty())
    {
        qDebug() << "角点影像为空";
        return;
    }
    Mat left, right;
    left = leftImage.clone();
    right = rightImage.clone();
}
