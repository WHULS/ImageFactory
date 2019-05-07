#include "FeatureWin.h"
#include "ui_FeatureWin.h"

// 比较函数，用于匹配点排序
// 升序排序
static bool asc_comp(MatchPoint m1, MatchPoint m2)
{
    return m1.cVal < m2.cVal;
}
// 降序排序
static bool dec_comp(MatchPoint m1, MatchPoint m2)
{
    return m1.cVal > m2.cVal;
}

FeatureWin::FeatureWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FeatureWin)
{
    ui->setupUi(this);

    // 数据初始化 /2. 数字摄影测量/imgs_for_digital_photogrammetry
    imageDir = "F:/杉/文章/大三下/2. 数字摄影测量/imgs_for_digital_photogrammetry";

    ui->statusBar->showMessage(imageDir);
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
    ui->statusBar->showMessage(tr("文件格式：") + rImagePath.section(".", -1, -1));

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
        ui->statusBar->showMessage("合并图像类型不一致");
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

        int height = cvRound(double(ui->Image_Left->geometry().height() * zoomScale)/100.0);
        int width = cvRound(double(ui->Image_Left->geometry().width() * zoomScale)/100.0);

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
    ui->statusBar->showMessage(QString().sprintf("图像尺寸：%d, %d", height, width));

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
    ui->statusBar->showMessage(QString().sprintf("图像尺寸：%d, %d", height, width));

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

    // 使用相关系数法寻找匹配点
    vector<MatchPoint> matchPoints;
    correlationMatch(leftImage.clone(), rightImage.clone(), leftCorner.clone(), rightCorner.clone(), matchPoints);
    this->matchPoints = matchPoints;

    // 降序排序
    sort(matchPoints.begin(), matchPoints.end(), dec_comp);

    // 在表格中显示匹配信息
    presentMatchInfo(matchPoints);
    // 绘制所有匹配线
    showMatchImage(leftImage.clone(), rightImage.clone(), matchPoints);
}
void FeatureWin::correlationMatch(Mat left, Mat right, Mat leftCorner, Mat rightCorner, vector<MatchPoint> &matchPoints)
{
    left.convertTo(left, CV_64F);
    right.convertTo(right, CV_64F);

    // 左影像的迭代器
    Mat_<uchar>::const_iterator lit, rit, litd, ritd;
    lit  = leftCorner. begin<uchar>();
    litd = leftCorner. end<uchar>();

    int leftWidth, rightWidth, leftHeight, rightHeight;
    leftHeight  = leftCorner .rows;
    leftWidth   = leftCorner .cols;
    rightHeight = rightCorner.rows;
    rightWidth  = rightCorner.cols;

    if (leftHeight != rightHeight || leftWidth != rightWidth)
    {
        showMessage("左右影像尺寸不一致");
        return;
    }

    // 构建搜索区，在搜索区内寻找相关系数最大的点作为匹配点
    int nx, ny;          // 计算窗口大小
    nx = ny = 9;

    int sx, sy;          // 搜索区大小
    sx = leftWidth/2;
    sy = leftHeight/2;

    int i, j, row_left, col_left, row_right, col_right;
    for (i=0; lit!=litd; lit++, i++)
    {
        if (!*lit) continue;

        row_left = i / leftWidth;
        col_left = i % leftWidth;

        // 放弃边缘点
        if (row_left-ny<0 || row_left+ny>=leftHeight ||
                col_left-nx<0 || col_left+nx>=leftWidth)
        {
            continue;
        }

        // 左窗口
        Mat leftWin = left(Range(row_left-ny, row_left+ny),
                           Range(col_left-nx, col_left+nx));

        // 搜索区（以左匹配点的位置为中心构建搜索区）
        int r1, r2, c1, c2;
        r1 = row_left-sy < 0 ? 0 : row_left-sy;
        r2 = row_left+sy > leftHeight ? leftHeight : row_left+sy;
        c1 = col_left-sx < 0 ? 0 : col_left-sx;
        c2 = col_left+sx > leftWidth ? leftWidth : col_left+sx;

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

            if ((row_right-ny<0 || row_right+ny>rightHeight) || // 匹配窗口越界
                (col_right-nx<0 || col_right+nx>rightWidth))
            {
                continue;
            }
            // 右窗口
            Mat rightWin = right(Range(row_right-ny, row_right+ny),
                               Range(col_right-nx, col_right+nx));

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

    // 统计匹配成功率
    if (QMessageBox::information(nullptr,
                                 tr("提示"),
                                 tr("是否为核线影像？"),
                                 QMessageBox::Yes,
                                 QMessageBox::No) == QMessageBox::Yes)
    {
        int count = 0;
        for (size_t i=0; i<matchPoints.size(); i++)
        {
            if (matchPoints[i].lpt.y == matchPoints[i].rpt.y)
            {
                count ++;
            }
        }
        QMessageBox::information(nullptr,
                                 tr("提示"),
                                 QString().sprintf("正确匹配点的个数：%d", count));
    }
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
    if (win1.type() != CV_64F) win1.convertTo(win1, CV_64F);
    if (win2.type() != CV_64F) win2.convertTo(win2, CV_64F);

    double sum12, sum1, sum2, sum11, sum22;
    sum12 = sum(win1.mul(win2))[0];
    sum1 = sum(win1)[0];
    sum2 = sum(win2)[0];
    sum11 = sum(win1.mul(win1))[0];
    sum22 = sum(win2.mul(win2))[0];
    double correlationValue;
    correlationValue = (sum12 - sum1 * sum2 / double(h1*w1))
        / sqrt((sum11 - sum1*sum1 / double(h1*w1))*(sum22 - sum2*sum2 / double(h1*w1)));

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

    QStringList strList;
    strList << "No" << "left X" << "left Y" << "right X" << "right Y" << "value";
    createTable(strList);
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
    if (leftCornerImage.empty()) left = leftImage.clone();
    else left = leftCornerImage.clone();
    if (rightCornerImage.empty()) right = rightImage.clone();
    else right = rightCornerImage.clone();

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
    vector<MatchPoint> matchPoints;

    Mat left, right;
    left = leftImage.clone();
    right = rightImage.clone();

    leastSquare(left, right, 100, matchPoints, 0.5);

    showMatchImage(leftImage.clone(), rightImage.clone(), matchPoints);

    // 按相关系数大小排序
    sort(matchPoints.begin(), matchPoints.end(), dec_comp);

    this->matchPoints = matchPoints;

    // 初始化表格
    initTable();
    presentMatchInfo(matchPoints);
}

/**
 * @brief FeatureWin::leastSquare 最小二乘影像匹配算法
 * @param left           左影像
 * @param right          右影像
 * @param anum           子区域数量
 * @param matchPoints    匹配点
 */
void FeatureWin::leastSquare(Mat left, Mat right, int anum, vector<MatchPoint> &matchPoints, double threshold)
{
    int leftHeight, leftWidth, rightHeight, rightWidth;
    leftHeight = left.rows;
    leftWidth = left.cols;
    rightHeight = right.rows;
    rightWidth = right.cols;
    if (leftHeight != rightHeight) exit(-1);
    if (leftWidth != rightWidth) exit(-1);

    // 转为灰度图
    if (left.channels() == 3) cvtColor(left, left, COLOR_BGR2GRAY);
    if (right.channels() == 3) cvtColor(right, right, COLOR_BGR2GRAY);

    // 转为双浮点精度
    left.convertTo(left, CV_64F);
    right.convertTo(right, CV_64F);

    // 重心化
    left = left - mean(left)[0];
    right = right - mean(right)[0];

    cout << sum(left)[0] << endl;
    cout << sum(right)[0] << endl;

    // 子窗口大小
    int winHeight, winWidth;
    winHeight = leftHeight / anum;
    winWidth = leftWidth / anum;

    // 计算每一个窗口的匹配点
    int i, j;
    for (i = 0; i < anum; i++)
    {
        j = 0;
    MatchWin2:
        for (; j < anum; j++)
        {
            Mat leftWin, rightWin;

            qDebug("行列(%d, %d), 当前点数：%d\n", i, j, matchPoints.size());
            ui->statusBar->showMessage(QString().sprintf("行列(%d, %d), 当前点数：%d\n", i, j, matchPoints.size()));
            // 直接得到左窗口
            int x10, y10;
            x10 = j * winWidth;
            y10 = i * winHeight;
            leftWin = left(Rect(x10, y10, winWidth, winHeight));

            // 灰度一阶差分算子
            Mat xKernel, yKernel;
            xKernel = (Mat_<double>(1, 3) << -0.5, 0, 0.5);
            yKernel = xKernel.t();

            // 左窗口的梯度
            Mat lx, ly;
            filter2D(leftWin, lx, CV_64F, xKernel);
            filter2D(leftWin, ly, CV_64F, yKernel);

            // 畸变系数
            double h0, h1, a0, a1, a2, b0, b1, b2;
            // 赋初值
            h0 = 0.0; h1 = 1.0;
            a0 = 0.0; a1 = 1.0; a2 = 0.0;
            b0 = 0.0; b1 = 0.0; b2 = 1.0;

            // 改正数
            double dh0, dh1, da0, da1, da2, db0, db1, db2;
            dh0 = dh1 = da0 = da1 = da2 = db0 = db1 = db2 = 999.0;

            int count = 0;
            int iterateCount = 10;
            while (abs(da0) >= threshold || abs(db0) >= threshold)
            {
                // 迭代超限，放弃影像
                if (count > iterateCount)
                {
                    j++;
                    goto MatchWin2;
                }

                // 右窗口左上角在右影像上的坐标
                int x20, y20;
                x20 = cvRound(a0 + a1 * x10 + a2 * y10);
                y20 = cvRound(b0 + b1 * x10 + b2 * y10);


                if (x20 < 0 || y20 < 0 ||
                    x20 + winWidth >= right.cols ||
                    y20 + winHeight >= right.rows)
                {
                    qDebug("窗口超出右影像范围: (%d, %d) (%d, %d)\n", x20, y20, x20 + winWidth, y20 + winHeight);
                    j++;
                    goto MatchWin2;
                }

                // 计算得到右窗口
                if (!rightWin.empty()) rightWin.release();
                rightWin.create(winHeight, winWidth, CV_64F);
                for (int m = y10; m < y10 + winHeight; m++)
                {
                    for (int n = x10; n < x10 + winWidth; n++)
                    {
                        // 计算右影像对应像素的位置
                        double x2, y2;
                        x2 = a0 + a1*double(n) + a2*double(m);
                        y2 = b0 + b1*double(n) + b2*double(m);

                        // 右影像超界，放弃该影像
                        if ((cvRound(x2) < 0 || cvRound(x2 + 1) >= right.cols) ||
                            (cvRound(y2) < 0 || cvRound(y2 + 1) >= right.rows) ||
                            (cvRound(x2 + 1) < 0 || cvRound(x2 + 1) >= right.cols) ||
                            (cvRound(y2 + 1) < 0 || cvRound(y2 + 1) >= right.rows))
                        {
                            j++;
                            qDebug("计算超界(%.3lf, %.3lf), (%.3lf, %.3lf)", x2, y2, x2 + winWidth, y2 + winHeight);
                            goto MatchWin2;
                        }

                        // 重采样 + 辐射改正
                        rightWin.at<double>(m - y10, n - x10) = h0 + h1 * resampleDouble(right, y2, x2);
                    }
                }

                // 计算灰度差分
                Mat gx, gy;
                filter2D(rightWin, gx, CV_64F, xKernel);
                filter2D(rightWin, gy, CV_64F, yKernel);

                // 逐点计算误差方程
                Mat C, L;
                C.create(winHeight * winWidth, 8, CV_64F);
                L.create(winHeight * winWidth, 1, CV_64F);
                for (int m = 0; m < winHeight; m++)
                {
                    for (int n = 0; n < winWidth; n++)
                    {
                        C.at<double>(m*winWidth + n, 0) = 1;
                        C.at<double>(m*winWidth + n, 1) = rightWin.at<double>(m, n);
                        C.at<double>(m*winWidth + n, 2) = gx.at<double>(m, n);
                        C.at<double>(m*winWidth + n, 3) = (n + x10)*gx.at<double>(m, n);
                        C.at<double>(m*winWidth + n, 4) = (m + y10)*gx.at<double>(m, n);
                        C.at<double>(m*winWidth + n, 5) = gy.at<double>(m, n);
                        C.at<double>(m*winWidth + n, 6) = (n + x10)*gy.at<double>(m, n);
                        C.at<double>(m*winWidth + n, 7) = (m + y10)*gy.at<double>(m, n);
                        L.at<double>(m*winWidth + n, 0) = leftWin.at<double>(m, n) - rightWin.at<double>(m, n);
                    }
                }

                // 解误差方程，得到改正数
                Mat X;
                X = (C.t() * C).inv()*C.t()*L;

                dh0 = X.at<double>(0, 0);
                dh1 = X.at<double>(1, 0);
                da0 = X.at<double>(2, 0);
                da1 = X.at<double>(3, 0);
                da2 = X.at<double>(4, 0);
                db0 = X.at<double>(5, 0);
                db1 = X.at<double>(6, 0);
                db2 = X.at<double>(7, 0);

                // qDebug("%d: %.3lf,%.3lf,%.3lf,%.3lf,%.3lf,%.3lf,%.3lf,%.3lf\n", count, dh0, dh1, da0, da1, da2, db0, db1, db2);

                double _h0, _h1, _a0, _a1, _a2, _b0, _b1, _b2;
                _h0 = h0;
                _h1 = h1;
                _a0 = a0;
                _a1 = a1;
                _a2 = a2;
                _b0 = b0;
                _b1 = b1;
                _b2 = b2;

                h0 = _h0 + dh0 + _h0*dh1;
                h1 = _h1 + _h1*dh1;
                a0 = _a0 + da0 + _a0*da1 + _b0*da2;
                a1 = _a1 + _a1*da1 + _b1*da2;
                a2 = _a2 + _a2*da1 + _b2*da2;
                b0 = _b0 + db0 + _a0*db1 + _b0*db2;
                b1 = _b1 + _a1*db1 + _b1*db2;
                b2 = _b2 + _a2*db1 + _b2*db2;

                // 迭代次数++
                count++;
            }

            // 计算左窗口的最佳匹配点位置
            int x1, y1;
            double sumx1, sumx2, sumy1, sumy2;
            sumx1 = sumx2 = sumy1 = sumy2 = 0.0; // 累加结果
            for (int m = 0; m < winHeight; m++)
            {
                for (int n = 0; n < winWidth; n++)
                {
                    sumx1 += (n + x10) * lx.at<double>(m, n) * lx.at<double>(m, n);
                    sumx2 += lx.at<double>(m, n) * lx.at<double>(m, n);
                    sumy1 += (m + y10) * ly.at<double>(m, n) * ly.at<double>(m, n);
                    sumy2 += ly.at<double>(m, n) * ly.at<double>(m, n);
                }
            }
            x1 = cvRound(sumx1 / sumx2 + 0.5);
            y1 = cvRound(sumy1 / sumy2 + 0.5);

            // 计算右窗口匹配点位置
            int x2, y2;
            x2 = cvRound(a0 + a1*x1 + a2*y1);
            y2 = cvRound(b0 + b1*x1 + b2*y1);

            if (count != 0)
            {
                matchPoints.push_back({
                    Point(x1, y1),
                    Point(x2, y2),
                    correlation(leftWin, rightWin)
                });
            }
        }
    }
}

/**
 * @brief FeatureWin::leastSquare 最小二乘影像匹配算法
 *        在image中根据win搜索匹配窗口
 * @param win          匹配窗口
 * @param image        影像
 * @param pt           影像上匹配点位置
 * @param cVal         相关系数
 * @param matchPoints  匹配点
 * @param h0 h1 a0 a1 a2 b0 b1 b2 畸变系数初值
 */
bool FeatureWin::leastSquare(Mat left, Mat right, Rect leftWinRect, Point &left_point, Point &right_point, double &cVal, double dr, double dc)
{

    // 影像尺寸
    int lr, lc, rr, rc;
    lr = left.rows;
    lc = left.cols;
    rr = right.rows;
    rc = right.cols;

    // 左窗口的左上角在左影像上的坐标
    int x10, y10;
    x10 = leftWinRect.x;
    y10 = leftWinRect.y;

    // 窗口尺寸
    int winHeight, winWidth;
    winHeight = leftWinRect.height;
    winWidth = leftWinRect.width;

    // 转为双浮点精度
    if (left.type() != CV_64F) left.convertTo(left, CV_64F);
    if (right.type() != CV_64F) right.convertTo(right, CV_64F);

    // 转为灰度图
    if (left.channels() == 3) cvtColor(left, left, COLOR_BGR2GRAY);
    if (right.channels() == 3) cvtColor(right, right, COLOR_BGR2GRAY);

    // 灰度一阶差分算子
    Mat xKernel, yKernel;
    xKernel = (Mat_<double>(1, 3) << -1, 0, 1);
    yKernel = xKernel.t();

    // 窗口
    Mat leftWin, rightWin;
    leftWin = left(leftWinRect);


    //Mat showLeft, showRight;
    //left.convertTo(showLeft, CV_8UC1);
    //right.convertTo(showRight, CV_8UC1);
    //rectangle(showLeft, leftWinRect, Scalar(255));
    //imshow("leftwin", showLeft);
    //waitKey(0);

    if (leftWin.empty())
    {
        qDebug("左窗口为空\n");
        return false;
    }

    // 畸变系数
    double h0, h1, a0, a1, a2, b0, b1, b2;
    h0 = 0.0; h1 = 1.0;
    a0 = dc; a1 = 1.0; a2 = 0.0;
    b0 = dr; b1 = 0.0; b2 = 1.0;

    // 改正数
    double dh0, dh1, da0, da1, da2, db0, db1, db2;
    dh0 = dh1 = da0 = da1 = da2 = db0 = db1 = db2 = 999.0;

    // 迭代计算影像上的匹配窗口
    int count = 0;
    int iterateCount = 10; // 迭代次数
    double threshold = 0.5;
    while (abs(da0) >= threshold || abs(db0) >= threshold)
    {
        // 迭代超限，放弃影像
        if (count > iterateCount)
        {
            return false;
        }

        // 右窗口的左上角在右影像上的坐标
        int x20, y20;
        x20 = cvRound(a0 + a1*double(x10) + a2*double(y10));
        y20 = cvRound(b0 + b1*double(x10) + b2*double(y10));

        // 窗口超出图像范围
        if (x20 < 0 || y20 < 0 ||
            x20 + winWidth >= right.cols ||
            y20 + winHeight >= right.rows)
        {
            qDebug("窗口超出右影像范围: (%d, %d) (%d, %d)\n", x20, y20, x20 + winWidth, y20 + winHeight);
            return false;
        }

        // 重采样获得右窗口
        if (!rightWin.empty()) rightWin.release();
        rightWin.create(winHeight, winWidth, CV_64F);
        for (int m = y10; m < y10 + winHeight; m++)
        {
            for (int n = x10; n < x10 + winWidth; n++)
            {
                // 计算出右窗口坐标
                double x2, y2;
                x2 = a0 + a1*double(n) + a2*double(m);
                y2 = b0 + b1*double(n) + b2*double(m);

                // 超界，放弃本次匹配
                if ((cvRound(y2) < 0 || cvRound(y2) >= right.rows) ||
                    (cvRound(x2) < 0 || cvRound(x2) >= right.cols) ||
                    (cvRound(y2 + 1.0) < 0 || cvRound(y2 + 1.0) >= right.rows) ||
                    (cvRound(x2 + 1.0) < 0 || cvRound(x2 + 1.0) >= right.cols))
                {
                    qDebug("计算点位超出右窗口范围: (x, y) = (%.3lf, %.3lf)", x2, y2);
                    qDebug("窗口: (%d, %d) (%d, %d)\n", x20, y20, x20 + winWidth, y20 + winHeight);
                    return false;
                }

                // 重采样 + 辐射改正
                rightWin.at<double>(m - y10, n - x10) = h0 + h1 * resampleDouble(right, y2, x2);
            }
        }

        //Mat showRightWin, showLeftWin;
        //leftWin.convertTo(showLeftWin, CV_8UC1);
        //rightWin.convertTo(showRightWin, CV_8UC1);
        //imshow("showLeftWin", showLeftWin);
        //imshow("showRightWin", showRightWin);
        //waitKey(0);

        // 右窗口的灰度差分
        Mat gx, gy;
        cv::filter2D(rightWin, gx, CV_64F, xKernel);
        cv::filter2D(rightWin, gy, CV_64F, yKernel);

        // 遍历右窗口，逐点计算误差方程
        Mat C, L;
        C.create(winHeight * winWidth, 8, CV_64F);
        L.create(winHeight * winWidth, 1, CV_64F);

        for (int m = 0; m < winHeight; m++)
        {
            for (int n = 0; n < winWidth; n++)
            {
                C.at<double>(m*winWidth + n, 0) = 1;
                C.at<double>(m*winWidth + n, 1) = rightWin.at<double>(m, n);
                C.at<double>(m*winWidth + n, 2) = gx.at<double>(m, n);
                C.at<double>(m*winWidth + n, 3) = (n + x10)*gx.at<double>(m, n);
                C.at<double>(m*winWidth + n, 4) = (m + y10)*gx.at<double>(m, n);
                C.at<double>(m*winWidth + n, 5) = gy.at<double>(m, n);
                C.at<double>(m*winWidth + n, 6) = (n + x10)*gy.at<double>(m, n);
                C.at<double>(m*winWidth + n, 7) = (m + y10)*gy.at<double>(m, n);
                L.at<double>(m*winWidth + n, 0) = leftWin.at<double>(m, n) - rightWin.at<double>(m, n);
            }
        }

        // 解误差方程，得到改正数
        Mat X;
        X = (C.t() * C).inv()*C.t()*L;

        dh0 = X.at<double>(0, 0);
        dh1 = X.at<double>(1, 0);
        da0 = X.at<double>(2, 0);
        da1 = X.at<double>(3, 0);
        da2 = X.at<double>(4, 0);
        db0 = X.at<double>(5, 0);
        db1 = X.at<double>(6, 0);
        db2 = X.at<double>(7, 0);

        double _h0, _h1, _a0, _a1, _a2, _b0, _b1, _b2;
        _h0 = h0;
        _h1 = h1;
        _a0 = a0;
        _a1 = a1;
        _a2 = a2;
        _b0 = b0;
        _b1 = b1;
        _b2 = b2;

        h0 = _h0 + dh0 + _h0*dh1;
        h1 = _h1 + _h1*dh1;
        a0 = _a0 + da0 + _a0*da1 + _b0*da2;
        a1 = _a1 + _a1*da1 + _b1*da2;
        a2 = _a2 + _a2*da1 + _b2*da2;
        b0 = _b0 + db0 + _a0*db1 + _b0*db2;
        b1 = _b1 + _a1*db1 + _b1*db2;
        b2 = _b2 + _a2*db1 + _b2*db2;

        // 迭代次数++
        count++;
    }

    // 计算相关系数
    cVal = correlation(leftWin, rightWin);
    if (cVal < 0.5) return false;

    // 左窗口的梯度
    Mat lx, ly;
    cv::filter2D(leftWin, lx, CV_64F, xKernel);
    cv::filter2D(leftWin, ly, CV_64F, yKernel);

    // 计算最佳匹配点位置(左)
    int x1, y1;
    double sumx1, sumx2, sumy1, sumy2;
    sumx1 = sumx2 = sumy1 = sumy2 = 0.0; // 累加结果
    for (int m = 0; m < winHeight; m++)
    {
        for (int n = 0; n < winWidth; n++)
        {
            sumx1 += (n + x10) * lx.at<double>(m, n) * lx.at<double>(m, n);
            sumx2 += lx.at<double>(m, n) * lx.at<double>(m, n);
            sumy1 += (m + y10) * ly.at<double>(m, n) * ly.at<double>(m, n);
            sumy2 += ly.at<double>(m, n) * ly.at<double>(m, n);
        }
    }
    x1 = cvRound(sumx1 / sumx2);
    y1 = cvRound(sumy1 / sumy2);
    left_point = Point(x1, y1);

    // 得到右影像上的匹配点
    int x2, y2;
    x2 = cvRound(a0 + a1*x1 + a2*y1);
    y2 = cvRound(b0 + b1*x1 + b2*y1);
    right_point = Point(x2, y2);

    return true;
}
double FeatureWin::resampleDouble(Mat image, double r, double c)
{
    int r1, r2, c1, c2;
    r1 = cvRound(r); r2 = r1 + 1;
    c1 = cvRound(c); c2 = c1 + 2;

    double dr, dc;
    dr = r - double(r1);
    dc = c - double(c1);

    double result = (1.0 - dr)*(1.0 - dc)*image.at<double>(r1, c1) + (1.0 - dr)*dc*image.at<double>(r1, c2) +
        dr*(1.0 - dc)*image.at<double>(r2, c1) + dr*dc*image.at<double>(r2, c2);

    return result;
}

void FeatureWin::on_least_square_plus_triggered()
{
    if (leftImage.empty() || rightImage.empty())
    {
        showMessage("影像为空");
        return;
    }
    if (leftCorner.empty() || rightCorner.empty())
    {
        showMessage("请先进行角点提取");
        return;
    }
    Mat left, right;
    left = leftImage.clone();
    right = rightImage.clone();

    if (left.channels() == 3) cvtColor(left, left, COLOR_BGR2GRAY);
    if (right.channels() == 3) cvtColor(right, right, COLOR_BGR2GRAY);

    Mat leftCorner, rightCorner;
    leftCorner = this->leftCorner.clone();
    rightCorner = this->rightCorner.clone();

    int lr, lc, rr, rc;
    lr = left.rows;
    lc = left.cols;
    rr = right.rows;
    rc = right.cols;
    if (lr != rr || lc != rc)
    {
        showMessage("左右影像尺寸不一致");
        return;
    }

    // 选取特征点
    // 1. 选取左侧特征点
    int centerRow, centerCol;
    centerRow = lr / 2;
    centerCol = lc / 2;

    // 第一对匹配点
    Point pt0, pt1;

    int centerSize;
    centerSize = 10;
    bool isSelected = false;
    while (!isSelected)
    {
        for (int i = centerRow - (centerSize / 2); i < centerRow + (centerSize / 2); i++)
        {
            for (int j = centerCol - (centerSize / 2); j < centerCol + (centerSize / 2); j++)
            {
                if (leftCorner.at<uchar>(i, j))
                {
                    pt0 = Point(j, i);
                    // 2. 在右侧获取匹配点
                    // 窗口尺寸
                    int winHeight, winWidth;
                    winHeight = winWidth = 50;

                    // 左窗口
                    Mat leftWin, rightWin;
                    Rect leftRect, rightRect;
                    leftRect = Rect(pt0.x - winWidth / 2, pt0.y - winHeight / 2, winWidth, winHeight);
                    leftWin = left(leftRect);
                    leftWin.convertTo(leftWin, CV_64F);

                    // 在左窗口画圆
                    Mat showLeft;
                    showLeft = left.clone();
                    circle(showLeft, pt0, 10, Scalar(255, 0, 0), 2);

                    // 遍历右影像，获得相关系数最大的那个窗口
                    Mat_<uchar>::const_iterator it = rightCorner.begin<uchar>();

                    double correlationValue = -1.0;
                    for (int i = 0; it != rightCorner.end<uchar>(); it++, i++)
                    {
                        if (*it)
                        {
                            int r, c;
                            r = i / rc;
                            c = i % rc;

                            // 排除边缘
                            if (c - winWidth / 2 < 0 || c + winWidth / 2 >= rc ||
                                r - winHeight / 2 < 0 || r + winHeight / 2 >= rr)
                            {
                                continue;
                            }

                            rightWin.release();
                            rightRect = Rect(c - winWidth / 2, r - winHeight / 2, winWidth, winHeight);
                            rightWin = right(rightRect);
                            rightWin.convertTo(rightWin, CV_64F);

                            // 计算相关系数
                            double cVal = correlation(leftWin, rightWin);
                            if (cVal > correlationValue)
                            {
                                correlationValue = cVal;
                                cout << correlationValue << endl;

                                pt1 = Point(c, r);
                            }
                        }
                    }

                    // 在右窗口画圆
                    Mat showRight;
                    showRight = right.clone();
                    circle(showRight, pt1, 10, Scalar(255, 0, 0), 2);

                    MatchPoint matchPoint = {pt0, pt1, 0.0};
                    showMatchImage(left, right, matchPoint);
                    showImage(showLeft, showRight);

                    // 确认
                    if (QMessageBox::information(nullptr,
                                                 tr("提示"),
                                                 tr("是否为正确匹配点？"),
                                                 QMessageBox::Yes,
                                                 QMessageBox::No) == QMessageBox::Yes)
                    {
                        isSelected = true;
                        goto HavePt0;
                    }
                }
            }
        }
        // 若找不到特征点，则扩大搜索范围
        centerSize += 10;
    }
    HavePt0:

    //    [597, 579]
    //    [605, 579]
    cout << pt0 << endl;
    cout << pt1 << endl;

    // 2. 记录行列差值，用于预测点位
    double dr, dc;
    dr = pt1.y - pt0.y;
    dc = pt1.x - pt0.x;

    // 3. 逐点匹配
    int winSize = 51;
    vector<MatchPoint> matchPoints;
    Mat_<uchar>::const_iterator it = leftCorner.begin<uchar>();

    left.convertTo(left, CV_64F);
    right.convertTo(right, CV_64F);

    for (int i = 0; it != leftCorner.end<uchar>(); it++, i++)
    {
        if (*it)
        {
            // 获取左影像特征点的行列号
            int r, c;
            r = i / lc;
            c = i % lc;

            qDebug("行列(%d, %d), 当前点数：%d\n", r, c, matchPoints.size());
            ui->statusBar->showMessage(QString().sprintf("行列(%d, %d), 当前点数：%d\n", r, c, matchPoints.size()));

            // 窗口超界
            if (r - (winSize + 1) / 2 < 0 || r + (winSize + 1) / 2 >= lr ||
                c - (winSize + 1) / 2 < 0 || c + (winSize + 1) / 2 >= lc)
            {
                continue;
            }

            // 3.1 以特征点为中心构建左窗口
            Rect leftWinRect;
            leftWinRect = Rect(c - winSize / 2, r - winSize / 2 , winSize, winSize);


            // 3.2 计算右影像上的匹配点坐标
            Point lpt, rpt;
            double correlationValue;
            if (leastSquare(left, right, leftWinRect, lpt, rpt, correlationValue, dr, dc))
            {
                matchPoints.push_back({ lpt, rpt, correlationValue });

                qDebug("(%d, %d), (%d, %d), %lf\n", lpt.x, lpt.y, rpt.x, rpt.y, correlationValue);
            }
        }
    }
    this->matchPoints = matchPoints;

    // 4. 显示匹配图
    left.convertTo(left, CV_8UC1);
    right.convertTo(right, CV_8UC1);
    showMatchImage(left, right, matchPoints);

    // 5. 匹配点排序
    sort(matchPoints.begin(), matchPoints.end(), dec_comp);

    // 6. 在表格中显示
    initTable();
    presentMatchInfo(matchPoints);
}

void FeatureWin::on_wallis_filter_triggered()
{
    if (leftImage.empty() || rightImage.empty())
    {
        showMessage("请先打开左右影像");
        return;
    }

    Mat left, right;
    left = leftImage.clone();
    right = rightImage.clone();

    // 获取灰度图
    Mat grayLeft, grayRight;
    grayLeft = left.clone();
    grayRight = right.clone();
    if (grayLeft.channels() == 3) cvtColor(grayLeft, grayLeft, COLOR_BGR2GRAY);
    if (grayRight.channels() == 3) cvtColor(grayRight, grayRight, COLOR_BGR2GRAY);

    // 双浮点矩阵
    Mat doubleLeft, doubleRight;
    left.convertTo(doubleLeft, CV_64F);
    right.convertTo(doubleRight, CV_64F);

    // Wallis 滤波
    Wallis wallisLeft(grayLeft, 0.5f, 0.5f, 127.0f, 50.0f);
    Wallis wallisRight(grayRight, 0.5f, 0.5f, 127.0f, 50.0f);

    Mat filterLeft, filterRight;
    filterLeft = grayLeft.clone();
    filterRight = grayRight.clone();

    wallisLeft.Wallisfilter(filterLeft);
    wallisRight.Wallisfilter(filterRight);

    showImage(filterLeft, filterRight);
    this->leftImage = filterLeft.clone();
    this->rightImage = filterRight.clone();
}
