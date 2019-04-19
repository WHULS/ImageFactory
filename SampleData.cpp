#include "SampleData.h"
#include "ui_SampleData.h"

// 声明静态成员
static Rect select;
static bool select_flag;
static Mat rectImage;
static SampleData *sd;

SampleData::SampleData(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SampleData)
{
    // 将 this 指针赋给静态成员，这样可以在静态成员函数中调用该实例的变量
    sd = this;

    ui->setupUi(this);
    CpDlg = new ControlPointDlg;

    // 设置第1栏 - 数据头
    QListView *dataList;
    dataList = ui->sampleDataList;
    // 设置为不可修改
    dataList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    dataListModel = new QStandardItemModel();
    // 将数据模型绑定到 dataList
    dataList->setModel(dataListModel);

    // 设置第2栏 - 数据信息
    QTableView *dataInfo;
    dataInfo = ui->dataInfo;
    dataInfoModel = new QStandardItemModel();
    dataInfo->setModel(dataInfoModel);

    // 填写项
    QStringList strList;
    strList << "当前像片";
    strList << "[数组]控制点";
    strList << "[数组]检校照片";
    for (int i=0; i<strList.size(); i++)
    {
        QString str = static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(str);
        dataListModel->appendRow(item);
    }

    // 绑定事件与槽
    connect(dataList, SIGNAL(clicked(QModelIndex)), this, SLOT(dataListClicked(QModelIndex)));

    QModelIndex currentIndex = dataListModel->index(2, 0);
    dataList->setCurrentIndex(currentIndex);
    emit dataList->clicked(currentIndex);

    on_open_calibration_info_triggered();
}

SampleData::~SampleData()
{
    currentImage.release();
    delete ui;
}

void SampleData::dataListClicked(int row)
{
    currentListNum = row;
    switch (row) {
    case 0:
    {
        QTableView *dataInfo;
        dataInfo = ui->dataInfo; dataInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dataInfoModel->clear();

        // 设置表头属性
        dataInfoModel->setHorizontalHeaderItem(0, new QStandardItem(tr("属性")));
        dataInfoModel->setHorizontalHeaderItem(1, new QStandardItem(tr("值")));
        dataInfo->setColumnWidth(0, 75);
        dataInfo->verticalHeader()->hide(); // 隐藏竖直的行号
        dataInfo->setSelectionBehavior(QAbstractItemView::SelectRows);

        if (!currentImage.empty())
        {
            dataInfoModel->setItem(0, 0, new QStandardItem("Rows"));
            dataInfoModel->setItem(0, 1, new QStandardItem(QString().sprintf("%d", currentImage.rows)));

            dataInfoModel->setItem(1, 0, new QStandardItem("Cols"));
            dataInfoModel->setItem(1, 1, new QStandardItem(QString().sprintf("%d", currentImage.cols)));

            dataInfoModel->setItem(2, 0, new QStandardItem("Path"));
            dataInfoModel->setItem(2, 1, new QStandardItem(caliImage.ImagePath));

            dataInfoModel->setItem(3, 0, new QStandardItem("CPoint"));
            dataInfoModel->setItem(3, 1, new QStandardItem(QString().sprintf("%d", caliImage.ControlPoints.size())));

            dataInfoModel->setItem(4, 0, new QStandardItem("X"));
            dataInfoModel->setItem(4, 1, new QStandardItem(QString().sprintf("%lf", caliImage.X)));

            dataInfoModel->setItem(5, 0, new QStandardItem("Y"));
            dataInfoModel->setItem(5, 1, new QStandardItem(QString().sprintf("%lf", caliImage.Y)));

            dataInfoModel->setItem(6, 1, new QStandardItem(QString().sprintf("%lf", caliImage.Z)));
            dataInfoModel->setItem(6, 0, new QStandardItem("Z"));

            dataInfoModel->setItem(7, 0, new QStandardItem("Phi"));
            dataInfoModel->setItem(7, 1, new QStandardItem(QString().sprintf("%lf", caliImage.Phi)));

            dataInfoModel->setItem(8, 0, new QStandardItem("Omega"));
            dataInfoModel->setItem(8, 1, new QStandardItem(QString().sprintf("%lf", caliImage.Omega)));

            dataInfoModel->setItem(9, 0, new QStandardItem("Kappa"));
            dataInfoModel->setItem(9, 1, new QStandardItem(QString().sprintf("%lf", caliImage.Kappa)));

            dataInfoModel->setItem(10, 0, new QStandardItem("x_0"));
            dataInfoModel->setItem(10, 1, new QStandardItem(QString().sprintf("%lf", caliImage.x_0)));

            dataInfoModel->setItem(11, 0, new QStandardItem("y_0"));
            dataInfoModel->setItem(11, 1, new QStandardItem(QString().sprintf("%lf", caliImage.y_0)));

            dataInfoModel->setItem(12, 0, new QStandardItem("f"));
            dataInfoModel->setItem(12, 1, new QStandardItem(QString().sprintf("%lf", caliImage.f)));

            showCurrentImage();
        }
        else
        {
            dataInfoModel->setItem(0, 0, new QStandardItem("null"));
            dataInfoModel->setItem(0, 1, new QStandardItem("暂无数据"));
        }
        // 调整列宽
        ui->dataInfo->resizeColumnsToContents();
        break;
    }
    case 1:
    {
        QTableView *dataInfo;
        dataInfo = ui->dataInfo; dataInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dataInfoModel->clear();

        dataInfoModel->setHorizontalHeaderItem(0, new QStandardItem(tr("点号")));
        dataInfoModel->setHorizontalHeaderItem(1, new QStandardItem(tr("X")));
        dataInfoModel->setHorizontalHeaderItem(2, new QStandardItem(tr("Y")));
        dataInfoModel->setHorizontalHeaderItem(3, new QStandardItem(tr("Z")));
        dataInfo->setColumnWidth(0, 75);
        dataInfo->verticalHeader()->hide(); // 隐藏竖直的行号
        dataInfo->setSelectionBehavior(QAbstractItemView::SelectRows);

        if (controlPoints.empty())
        {
            dataInfoModel->setItem(0, 0, new QStandardItem("null"));
            dataInfoModel->setItem(0, 1, new QStandardItem("null"));
            dataInfoModel->setItem(0, 2, new QStandardItem("null"));
            dataInfoModel->setItem(0, 3, new QStandardItem("null"));
        }
        else
        {
            for (size_t i=0; i<controlPoints.size(); i++)
            {
                dataInfoModel->setItem(int(i), 0, new QStandardItem(QString().sprintf("%d", controlPoints[i].num)));
                dataInfoModel->setItem(int(i), 1, new QStandardItem(QString().sprintf("%lf", controlPoints[i].X)));
                dataInfoModel->setItem(int(i), 2, new QStandardItem(QString().sprintf("%lf", controlPoints[i].Y)));
                dataInfoModel->setItem(int(i), 3, new QStandardItem(QString().sprintf("%lf", controlPoints[i].Z)));
            }
        }
        // 调整列宽
        ui->dataInfo->resizeColumnsToContents();
        break;
    }
    case 2:
    {
        QTableView *dataInfo;
        dataInfo = ui->dataInfo; dataInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dataInfoModel->clear();

        dataInfoModel->setHorizontalHeaderItem(0, new QStandardItem(tr("像片名")));
        dataInfoModel->setHorizontalHeaderItem(1, new QStandardItem(tr("控制点数")));
        dataInfoModel->setHorizontalHeaderItem(2, new QStandardItem(tr("X")));
        dataInfoModel->setHorizontalHeaderItem(3, new QStandardItem(tr("Y")));
        dataInfoModel->setHorizontalHeaderItem(4, new QStandardItem(tr("Z")));
        dataInfoModel->setHorizontalHeaderItem(5, new QStandardItem(tr("φ")));
        dataInfoModel->setHorizontalHeaderItem(6, new QStandardItem(tr("ω")));
        dataInfoModel->setHorizontalHeaderItem(7, new QStandardItem(tr("κ")));
        dataInfoModel->setHorizontalHeaderItem(8, new QStandardItem(tr("x_0")));
        dataInfoModel->setHorizontalHeaderItem(9, new QStandardItem(tr("y_0")));
        dataInfoModel->setHorizontalHeaderItem(10, new QStandardItem(tr("f")));
        dataInfo->verticalHeader()->hide(); // 隐藏竖直的行号
        dataInfo->setSelectionBehavior(QAbstractItemView::SelectRows);

        if (controlPoints.empty())
        {
            dataInfoModel->setItem(0, 0, new QStandardItem("null"));
            dataInfoModel->setItem(0, 1, new QStandardItem("null"));
            dataInfoModel->setItem(0, 2, new QStandardItem("null"));
            dataInfoModel->setItem(0, 3, new QStandardItem("null"));
            dataInfoModel->setItem(0, 4, new QStandardItem("null"));
            dataInfoModel->setItem(0, 5, new QStandardItem("null"));
            dataInfoModel->setItem(0, 6, new QStandardItem("null"));
            dataInfoModel->setItem(0, 7, new QStandardItem("null"));
            dataInfoModel->setItem(0, 8, new QStandardItem("null"));
            dataInfoModel->setItem(0, 9, new QStandardItem("null"));
            dataInfoModel->setItem(0, 10, new QStandardItem("null"));
        }
        else
        {
            for (size_t i=0; i<caliImages.size(); i++)
            {
                dataInfoModel->setItem(int(i), 0, new QStandardItem(caliImages[i].ImagePath.section("/", -1, -1)));
                dataInfoModel->setItem(int(i), 1, new QStandardItem(QString().sprintf("%d", caliImages[i].ControlPoints.size())));
                dataInfoModel->setItem(int(i), 2, new QStandardItem(QString().sprintf("%lf", caliImages[i].X)));
                dataInfoModel->setItem(int(i), 3, new QStandardItem(QString().sprintf("%lf", caliImages[i].Y)));
                dataInfoModel->setItem(int(i), 4, new QStandardItem(QString().sprintf("%lf", caliImages[i].Z)));
                dataInfoModel->setItem(int(i), 5, new QStandardItem(QString().sprintf("%lf", caliImages[i].Phi)));
                dataInfoModel->setItem(int(i), 6, new QStandardItem(QString().sprintf("%lf", caliImages[i].Omega)));
                dataInfoModel->setItem(int(i), 7, new QStandardItem(QString().sprintf("%lf", caliImages[i].Kappa)));
                dataInfoModel->setItem(int(i), 8, new QStandardItem(QString().sprintf("%lf", caliImages[i].x_0)));
                dataInfoModel->setItem(int(i), 9, new QStandardItem(QString().sprintf("%lf", caliImages[i].y_0)));
                dataInfoModel->setItem(int(i), 10, new QStandardItem(QString().sprintf("%lf", caliImages[i].f)));
            }
        }
        // 调整列宽
        ui->dataInfo->resizeColumnsToContents();
        break;
    }
    default:
        break;
    }
}

void SampleData::dataListClicked(QModelIndex index)
{
//    qDebug() << index.data().toString(); // TODO: 解决显示的中文乱码
    dataListClicked(index.row());
}

void SampleData::showCpDlg()
{
    CpDlg->show();
}

bool SampleData::controlPointEmpty()
{
    if (controlPoints.empty())
        return true;
    else
        return false;
}
bool SampleData::caliImageEmpty()
{
    if (caliImages.empty())
        return true;
    else
        return false;
}

// 添加控制点，返回添加的位置
size_t SampleData::pushControlPoint(double x, double y, double z, int num)
{
    CPoint cp = {x, y, z, num};
    CpDlg->pushControlPoint(cp); // 存入对话框
    controlPoints.push_back(cp);
    return controlPoints.size()-1;
}
size_t SampleData::pushControlPoint(CPoint cp)
{
    controlPoints.push_back(cp);
    CpDlg->pushControlPoint(cp); // 存入对话框
    return controlPoints.size()-1;
}
// 添加影像，返回添加的位置
size_t SampleData::pushSampleData(CaliImage caliImage)
{
    caliImages.push_back(caliImage);
    return caliImages.size()-1;
}

// 获取标号为num的控制点
bool SampleData::getControlPoint(int num, CPoint *cp)
{
    if (controlPoints.empty())
    {
        qDebug() << "控制点为空";
        return false;
    }
    else
    {
        for (size_t i=0; i<controlPoints.size(); i++)
        {
            if (controlPoints[i].num == num)
            {
                *cp = controlPoints[i];
                return true;
            }
        }
        qDebug() << "找不到控制点";
        return false;
    }
}

// 获取数组中第num幅影像
bool SampleData::getSampleData(size_t num, Mat *img)
{
    if (caliImages.empty())
    {
        qDebug() << "数组为空";
        return false;
    }
    else
    {
        // 超出图像范围
        if (num > caliImages.size()-1)
            return false;

        QString imagePath = caliImages[num].ImagePath;
        Mat readImage = imread(imagePath.toLocal8Bit().data(), 1);
        if (!readImage.empty() || readImage.data)
        {
            *img = readImage;
            return true;
        }
        else
        {
            qDebug() << "图像打开失败";
            return false;
        }
    }

}

// 获取控制点数量
int SampleData::controlPointNumber()
{
    return int(controlPoints.size());
}
// 获取影像数量
int SampleData::caliImageNumber()
{
    return int(caliImages.size());
}

// 清空控制点
bool SampleData::clearControlPoint()
{
    controlPoints.clear();
    CpDlg->clearControlPoint();
    return true;
}
// 清空影像
bool SampleData::clearSampleData()
{
    caliImages.clear();
    return true;
}

// 绘制十字
void SampleData::drawCross(Mat img, Point center, int size, int thickness,  Scalar color)
{
    // 绘制横线
    line(img,Point(center.x-size/2,center.y),Point(center.x+size/2,center.y),color,thickness);
    // 绘制竖线
    line(img,Point(center.x,center.y-size/2),Point(center.x,center.y+size/2),color,thickness);

}

// 在指定区域检测椭圆
bool SampleData::detectEllipse(Mat roiImg)
{
    Mat thrImg, showImg;

    showImg = roiImg.clone();
    cvtColor(showImg, thrImg, COLOR_BGR2GRAY);
    threshold(thrImg, thrImg, 100, 255, THRESH_BINARY);

    vector<vector<Point>> contours;
    findContours(thrImg, contours, RETR_LIST, CHAIN_APPROX_NONE);

    RotatedRect box;
    size_t i;
    for (i=0; i<contours.size(); i++)
    {
        double area = contourArea(contours[i]);
        if (area < 1e2 || 1e5 < area) continue;
        if (contours[i].size() < 100) continue;

        box = fitEllipse(contours[i]);

        ellipse(showImg, box, Scalar(0, 255, 0), 2);

        // 只画一个椭圆，一般第一个椭圆是ROI中最下方的那个
        break;
    }
    // 在框选图像上画十字
    drawCross(showImg, box.center, 30);

    // 显示框选图像
    namedWindow("Is it OK?", WINDOW_NORMAL);
    imshow("Is it OK?", showImg);

    if (QMessageBox::information(nullptr,
                             QObject::tr("提示"),
                             QString().sprintf("控制点中心坐标为 (%lf, %lf)", select.x + box.center.x, select.y + box.center.y),
                             QMessageBox::Yes,
                             QMessageBox::No)==QMessageBox::Yes)
    {
        // 在原图上画椭圆
        RotatedRect ellipseBox = box;
        ellipseBox.center.x += select.x;
        ellipseBox.center.y += select.y;

        ellipse(rectImage, ellipseBox, Scalar(0, 255, 0), 2);

        // 十字标
        drawCross(rectImage, ellipseBox.center, 30, 2);
        imshow("Current Image", rectImage);

        // 选择控制点
        SelectCpDlg *sCpDlg = new SelectCpDlg(currentCPtNum);
        sCpDlg->show();

        int exitNum = sCpDlg->exec();
        if (exitNum != -1)
        {
            CPoint cp;
            currentCPtNum = exitNum;
            if (getControlPoint(currentCPtNum, &cp))
            {
                cp.x = double(ellipseBox.center.x);
                cp.y = double(ellipseBox.center.y);
                cp.num = currentCPtNum;

                // 存入影像数组中
                caliImage.ControlPoints.push_back(cp);

                if (imageIndex != -1)
                {
                    caliImages[imageIndex].ControlPoints.push_back(cp);
                    renewData(2);
                }

                currentCPtNum++;
                renewData(0);
            }
            else
            {
                QMessageBox::information(nullptr,
                                         QObject::tr("提示"),
                                         QObject::tr("控制点读取错误"));
            }

        }

        destroyWindow("Is it OK?");
    }
    else
    {
        destroyWindow("Is it OK?");
    }

    return true;
}

// 鼠标事件监听
static void CPointMouseClick(int event, int x, int y, int flags, void *params)
{
    switch (event)
    {
    // 右键点击选取 ROI
    case EVENT_RBUTTONDOWN:
    {
        select.x = x;
        select.y = y;
        select_flag = true;
        break;
    }
    case EVENT_MOUSEMOVE:
    {
        if (select_flag)
        {
            if (sd->controlPoints.size() == 0)
            {
                QMessageBox::information(nullptr,
                                         QObject::tr("提示"),
                                         QObject::tr("请先读取控制点"));
                select_flag = false;
                break;
            }
            Point p1, p2;
            sd->currentImage.copyTo(rectImage);
            p1 = Point(select.x, select.y);
            p2 = Point(x, y);
            rectangle(rectImage, p1, p2, Scalar(0, 255, 0), 2);
            imshow("Current Image", rectImage);
        }
        break;
    }
    case EVENT_RBUTTONUP:
    {
        select_flag = false;
        //显示框出的ROI
        Rect roi = Rect(Point(select.x, select.y), Point(x, y));

        // 坐标转换，保证select为左上角的点
        select.x = select.x > x ? x : select.x;
        select.y = select.y > y ? y : select.y;

        if (roi.width && roi.height)
        {
            // 获取ROI影像
            Mat roiImg = sd->currentImage(roi);
            // 在ROI影像上检测椭圆
            sd->detectEllipse(roiImg);
        }
        break;
    }
    // 双击保存
    case EVENT_LBUTTONDBLCLK:
    {
        if (QMessageBox::information(sd,
                                    sd->tr("提示"),
                                    sd->tr("是否保存当前影像并打开下一幅？"),
                                    QMessageBox::Yes,
                                    QMessageBox::No) == QMessageBox::Yes)
        {
            // 存入数组
            if (sd->imageIndex == -1)
                sd->caliImages.push_back(sd->caliImage);
            else
                sd->caliImages[sd->imageIndex] = sd->caliImage;
            // 清空
            sd->caliImage.clear();
            sd->currentCPtNum=0;
            // 打开新影像
            sd->on_open_image_triggered();
            // 刷新信息表
            sd->renewData(2);
        }
        
        break;
    }
    default:
    {
        break;
    }
    }
}

void SampleData::on_read_control_point_triggered()
{
    if (controlPointEmpty())
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
                fscanf(fp, "%d %lf %lf %lf %d\n %d\n", &cPoint.num, &cPoint.X, &cPoint.Y, &cPoint.Z, &t, &t);

                // cout << cPoint.num << ": " << cPoint.X << ", " << cPoint.Y << ", " << cPoint.Z << endl;
                pushControlPoint(cPoint);
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
            clearControlPoint();
            ui->read_control_point->setText(tr("读取控制点文件(&O)"));
        }
    }

    // 刷新视窗
    renewData(1);
}

void SampleData::on_open_image_triggered()
{
    // 获取文件路径
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    this->imageDir,
                                                    tr("Image File(*.png *.jpg *.jpeg *.bmp *.raw)"));
    this->imageDir = fileName.section("/", 0, -2); // 保存图像位置，作为下次打开的根目录
    this->imagePath = fileName;
    if (fileName.isEmpty()) return;
    

    Mat readImage = imread(imagePath.toLocal8Bit().data(), 1);
    qDebug() << imagePath;
    if (!readImage.empty() || readImage.data)
    {
        // 更新当前影像
        currentImage.release();
        currentImage = readImage;
        caliImage.clear();
        caliImage.ImagePath = imagePath;

        // 标记为非图像数组内像片
        imageIndex = -1;

        showCurrentImage();

        // 刷新视窗
        renewData(0);
    }
    else
    {
        qDebug() << "像片打开失败";
    }
}

void SampleData::renewData(int idx)
{
    if (ui->sampleDataList->currentIndex().row() == idx)
    {
        dataListClicked(idx);
    }
    
}

// 指定图像上画控制点
void SampleData::showControlPoint(Mat cPtImage, vector<CPoint> points, String winName)
{
    Mat crossImage = cPtImage.clone();
    Point center;
    for (size_t i=0; i<points.size(); i++)
    {
        // 画十字标
        center = Point(points[i].x, points[i].y);
        drawCross(crossImage, center, 100, 3);
        // 画点号
        putText(crossImage, to_string(points[i].num), Point(center.x+10, center.y-10),
                FONT_HERSHEY_SCRIPT_SIMPLEX, 5.0, Scalar(255, 0, 0), 5);
    }
    namedWindow(winName, WINDOW_NORMAL);
    imshow(winName, crossImage);
}

void SampleData::on_dataInfo_clicked(const QModelIndex &index)
{
    // 表头字符串
    // QString columnHeaderString =  dataInfoModel->horizontalHeaderItem(index.column())->text();

    on_dataInfo_activated(index);
}

// 保存信息到 xml 文件
void SampleData::on_save_calibration_info_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("保存"),
                                                    xmlDir,
                                                    tr("_03sd文件(*._03sd)"));

    qDebug() << fileName;
    if (fileName.isEmpty()) return;

    xmlDir = fileName.section("/", 0, -2);

    // 打开文件
    QFile file(fileName);
    file.open(QIODevice::ReadWrite);

    // 文件实例
    QDomDocument doc;
    // 头信息
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"GB2312\"");
    doc.appendChild(instruction);
    // 根节点
    QDomElement root = doc.createElement("Sample-Data");
    doc.appendChild(root);

    // 控制点
    QDomElement cPts = doc.createElement("Control-Points");

    QDomElement controlPointNumber = doc.createElement("Control-Point-Number");
    controlPointNumber.appendChild(doc.createTextNode(QString().sprintf("%d", int(controlPoints.size()))));
    cPts.appendChild(controlPointNumber);

    for (size_t i=0; i<controlPoints.size(); i++)
    {
        QDomElement cPt = doc.createElement("Control-Point");

        // 点号
        QDomElement numEl = doc.createElement("Number");
        // 世界坐标
        QDomElement xEl = doc.createElement("X");
        QDomElement yEl = doc.createElement("Y");
        QDomElement zEl = doc.createElement("Z");

        QDomText numTx = doc.createTextNode(QString().sprintf("%d", controlPoints[i].num));
        numEl.appendChild(numTx);
        QDomText xTx = doc.createTextNode(QString().sprintf("%.12lf", controlPoints[i].X));
        xEl.appendChild(xTx);
        QDomText yTx = doc.createTextNode(QString().sprintf("%.12lf", controlPoints[i].Y));
        yEl.appendChild(yTx);
        QDomText zTx = doc.createTextNode(QString().sprintf("%.12lf", controlPoints[i].Z));
        zEl.appendChild(zTx);

        cPt.appendChild(numEl);
        cPt.appendChild(xEl);
        cPt.appendChild(yEl);
        cPt.appendChild(zEl);

        cPts.appendChild(cPt);
    }

    root.appendChild(cPts);

    // 检校像片
    QDomElement caliImgs = doc.createElement("Calibration-Images");

    for (size_t i=0; i<caliImages.size(); i++)
    {
        QDomElement caliImg = doc.createElement("Calibration-Image");

        QDomElement imgPath = doc.createElement("Image-Path");
        QDomElement worldX = doc.createElement("X");
        QDomElement worldY = doc.createElement("Y");
        QDomElement worldZ = doc.createElement("Z");
        QDomElement phi = doc.createElement("Phi");
        QDomElement omega = doc.createElement("Omega");
        QDomElement kappa = doc.createElement("Kappa");
        QDomElement x_0 = doc.createElement("x_0");
        QDomElement y_0 = doc.createElement("y_0");
        QDomElement f = doc.createElement("f");
        QDomElement cPts = doc.createElement("Control-Points");
        QDomElement cPtNum = doc.createElement("Control-Point-Number");

        imgPath.appendChild(doc.createTextNode(caliImages[i].ImagePath));
        worldX.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].X)));
        worldY.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].Y)));
        worldZ.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].Z)));
        phi.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].Phi)));
        omega.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].Omega)));
        kappa.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].Kappa)));
        x_0.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].x_0)));
        y_0.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].y_0)));
        f.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].f)));
        // 每张影像的控制点个数
        size_t ptNum = caliImages[i].ControlPoints.size();
        cPtNum.appendChild(doc.createTextNode(QString().sprintf("%d", int(ptNum))));
        cPts.appendChild(cPtNum);
        // 为每张影像填充控制点
        for (size_t j=0; j<ptNum; j++)
        {
            QDomElement cPt = doc.createElement("Control-Point");

            QDomElement ptNo = doc.createElement("No");
            QDomElement X = doc.createElement("X");
            QDomElement Y = doc.createElement("Y");
            QDomElement Z = doc.createElement("Z");
            QDomElement x = doc.createElement("x");
            QDomElement y = doc.createElement("y");

            ptNo.appendChild(doc.createTextNode(QString().sprintf("%d", caliImages[i].ControlPoints[j].num)));
            X.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].ControlPoints[j].X)));
            Y.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].ControlPoints[j].Y)));
            Z.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].ControlPoints[j].Z)));
            x.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].ControlPoints[j].x)));
            y.appendChild(doc.createTextNode(QString().sprintf("%.12lf", caliImages[i].ControlPoints[j].y)));

            cPt.appendChild(ptNo);
            cPt.appendChild(X);
            cPt.appendChild(Y);
            cPt.appendChild(Z);
            cPt.appendChild(x);
            cPt.appendChild(y);

            cPts.appendChild(cPt);
        }

        caliImg.appendChild(imgPath);
        caliImg.appendChild(worldX);
        caliImg.appendChild(worldY);
        caliImg.appendChild(worldZ);
        caliImg.appendChild(phi);
        caliImg.appendChild(omega);
        caliImg.appendChild(kappa);
        caliImg.appendChild(x_0);
        caliImg.appendChild(y_0);
        caliImg.appendChild(f);
        caliImg.appendChild(cPts);

        caliImgs.appendChild(caliImg);
    }
    root.appendChild(caliImgs);

    // 文件保存
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();

    QMessageBox::information(this,
                             tr("成功"),
                             tr("保存成功！"));
}

void SampleData::on_open_calibration_info_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("打开数据文件"),
                                                    xmlDir,
                                                    tr("_03sd文件(*._03sd) ;; 所有文件(*.*)"));
    qDebug() << fileName;
    if (fileName.isEmpty()) return;

    xmlDir = fileName.section("/", 0, -2);
    this->H.clear();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        qDebug()<<"open for read error..." ;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;

    // 将xml文件读入doc树
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        qDebug()<<"setcontent error..." ;
        file.close();
    }
    file.close();

    // 获取根元素
    QDomElement root = doc.documentElement();
    if (root.tagName() != "Sample-Data")
    {
       qDebug() << "root.tagName != Sample-Data...";
       QMessageBox::information(this,
                                       tr("提示"),
                                       tr("请打开正确的xml文件"));
       return;
    }

    // 第一个节点 <Control-Points>
    QDomElement firstEl = root.firstChildElement("Control-Points");
    QDomNode cpNode = firstEl.firstChild();
    // 控制点个数
    qDebug() << "控制点个数：" << cpNode.toElement().text().toInt();
    cpNode = cpNode.nextSibling();
    // 清空原有控制点信息
    controlPoints.clear();
    // 遍历控制点
    while (!cpNode.isNull())
    {
        QDomElement e1, e2, e3, e4;
        e1 = cpNode.firstChildElement("Number");
        e2 = cpNode.firstChildElement("X");
        e3 = cpNode.firstChildElement("Y");
        e4 = cpNode.firstChildElement("Z");

        qDebug() << e1.text() << e2.text() << e3.text() << e4.text();

        // 插入控制点数组
        CPoint cp = {e2.text().toDouble(), e3.text().toDouble(), e4.text().toDouble(), e1.text().toInt(), -1, -1};
        controlPoints.push_back(cp);

        cpNode = cpNode.nextSibling();
    }
    renewData(1);
    // 第二个节点 <Calibration-Images>
    QDomElement secondEl = root.firstChildElement("Calibration-Images");
    QDomNode imgNode = secondEl.firstChild();
    // 清空原有影像
    caliImages.clear();
    while (!imgNode.isNull())
    {
        QDomElement e1,e2,e3,e4,e5,e6,e7,e8,e9,e10,e11;
        e1 = imgNode.firstChildElement("Image-Path");
        e2 = imgNode.firstChildElement("X");
        e3 = imgNode.firstChildElement("Y");
        e4 = imgNode.firstChildElement("Z");
        e5 = imgNode.firstChildElement("Phi");
        e6 = imgNode.firstChildElement("Omega");
        e7 = imgNode.firstChildElement("Kappa");
        e8 = imgNode.firstChildElement("x_0");
        e9 = imgNode.firstChildElement("y_0");
        e10 = imgNode.firstChildElement("f");
        e11 = imgNode.firstChildElement("Control-Points");

        CaliImage caliImg;
        caliImg.ImagePath    = e1.text();
        caliImg.X            = e2.text().toDouble();
        caliImg.Y            = e3.text().toDouble();
        caliImg.Z            = e4.text().toDouble();
        caliImg.Phi          = e5.text().toDouble();
        caliImg.Omega        = e6.text().toDouble();
        caliImg.Kappa        = e7.text().toDouble();
        caliImg.x_0          = e8.text().toDouble();
        caliImg.y_0          = e9.text().toDouble();
        caliImg.f            = e10.text().toDouble();

        QDomNode imgCpNode = e11.firstChild();
        qDebug() << "图片【" << e1.text() << "】的控制点个数为："
                 << imgCpNode.toElement().text().toInt();
        imgCpNode = imgCpNode.nextSibling();
        while (!imgCpNode.isNull())
        {
            QDomElement e1,e2,e3,e4,e5,e6;
            e1 = imgCpNode.firstChildElement("No");
            e2 = imgCpNode.firstChildElement("X");
            e3 = imgCpNode.firstChildElement("Y");
            e4 = imgCpNode.firstChildElement("Z");
            e5 = imgCpNode.firstChildElement("x");
            e6 = imgCpNode.firstChildElement("y");

            CPoint cp = {e2.text().toDouble(), e3.text().toDouble(), e4.text().toDouble(),
                        e1.text().toInt(),
                        e5.text().toDouble(),e6.text().toDouble()};
            caliImg.ControlPoints.push_back(cp);

            imgCpNode = imgCpNode.nextSibling();
        }
        caliImages.push_back(caliImg);
        imgNode = imgNode.nextSibling();
    }
    renewData(2);
}

void SampleData::showCurrentImage()
{
    namedWindow("Current Image", WINDOW_NORMAL);
    imshow("Current Image", currentImage);
    setMouseCallback("Current Image", CPointMouseClick);
}

void SampleData::on_dataInfo_activated(const QModelIndex &index)
{
    int row, col;
    row = index.row();
    col = index.column();
    // 当点击的是“当前影像”一栏的“控制点(row 3)”时
    if (currentListNum==0 && row == 3)
    {
        showControlPoint(currentImage, caliImage.ControlPoints);
    }
    else if (currentListNum==2)
    {
        this->currentCPtNum = 0;
        caliImageChanged(row, col);
    }
}

void SampleData::caliImageChanged(int row, int col)
{
    if (row >= int(caliImages.size())) return;
    caliImage = caliImages[size_t(row)];
    // 标记影像
    imageIndex = row;
    currentImage.release();
    currentImage = imread(caliImage.ImagePath.toLocal8Bit().data(), 1);
    if (currentImage.empty())
    {
        qDebug() << "图像为空";
        showMessage("图像为空，请确认路径无误");
    }
    else
    {
        // 更新图像信息
        renewData(0);
        // 由于renewData只针对已显示的list更新，因此需要手动更新影像
        showCurrentImage();

        // 点下控制点所在列
        if (col == 1)
        {
            showControlPoint(currentImage, caliImage.ControlPoints);
        }
    }
}

void SampleData::on_calculate_dlt_param_triggered()
{
    if (caliImage.isEmpty())
    {
        showMessage("影像为空");
        return;
    }

    if (caliImage.ControlPoints.size() < 4)
    {
        showMessage("请至少选取4个控制点");
        return;
    }

    vector<CPoint> cPts = caliImage.ControlPoints;
    size_t cpNum = cPts.size();
    showMessage(QString().sprintf("控制点个数：%d", cpNum));

    // 系数矩阵A
    Matrix paramMatrix_A(cpNum*2, 8);
    // 观测值X
    Matrix observedValue_X(cpNum*2, 1);

    size_t i,j;

    // 将控制点的单位转化成mm
//    double resolutionX, resolutionY;
//    resolutionX = resolutionY = 300; // dpi
//    double pixelSizeX = 25.400 / resolutionX; // 像素大小(mm)
//    double pixelSizeY = 25.400 / resolutionY;
//    for (i=0; i<cpNum; i++)
//    {
//        cPts[i].x *= pixelSizeX;
//        cPts[i].y *= pixelSizeY;
//    }
//    qDebug() << "像素大小" << pixelSizeX << ", " << pixelSizeY << "mm";

    for (i=0,j=0; i<cpNum*2&&j<cpNum; i+=2, j++)
    {
        paramMatrix_A[i][0] = cPts[j].X;
        paramMatrix_A[i][1] = cPts[j].Y;
        paramMatrix_A[i][2] = 1;
        paramMatrix_A[i][3] = 0;
        paramMatrix_A[i][4] = 0;
        paramMatrix_A[i][5] = 0;
        paramMatrix_A[i][6] = cPts[j].x * cPts[j].X;
        paramMatrix_A[i][7] = cPts[j].x * cPts[j].Y;

        paramMatrix_A[i+1][0] = 0;
        paramMatrix_A[i+1][1] = 0;
        paramMatrix_A[i+1][2] = 0;
        paramMatrix_A[i+1][3] = cPts[j].X;
        paramMatrix_A[i+1][4] = cPts[j].Y;
        paramMatrix_A[i+1][5] = 1;
        paramMatrix_A[i+1][6] = cPts[j].y * cPts[j].X;
        paramMatrix_A[i+1][7] = cPts[j].y * cPts[j].Y;

        observedValue_X[i][0] = cPts[j].x;
        observedValue_X[i+1][0] = cPts[j].y;
    }
//    paramMatrix_A.print();
//    observedValue_X.print();

    Matrix result_H = (paramMatrix_A.transposition()*paramMatrix_A).reverse()*paramMatrix_A.transposition()*observedValue_X;
//    result_H.print();

    // 保存dlt参数
    vector<double> H;
    if (!result_H.toVector(&H)) exit(-1);
    this->H.push_back(H);


    // 检验结果
    Matrix calculatedVelue_X = paramMatrix_A*result_H;
    Matrix V = calculatedVelue_X - observedValue_X;
    double sigma_0 = sqrt((V.transposition()*V/(2*cpNum - 8))[0][0]);
    qDebug() << "单位权中误差: " << sigma_0 << "mm";
    showMessage(QString().sprintf("单位权中误差：%lf mm", sigma_0));

    // 绘制计算结果
    Point center1, center2;
    vector<CPoint> points = caliImage.ControlPoints;
    Mat showImage = currentImage.clone();
    for (i=0; i<cpNum; i++)
    {
        center2 = Point(points[i].x, points[i].y);
        drawCross(showImage, center2, 100, 3);
        center1 = Point(int(calculatedVelue_X[2*i][0]), int(calculatedVelue_X[2*i+1][0]));
        circle(showImage, center1, 3, Scalar(255, 255, 0));
        // 画点号
        putText(showImage, to_string(points[i].num), Point(center2.x+10, center2.y-10),
                FONT_HERSHEY_SCRIPT_SIMPLEX, 5.0, Scalar(255, 0, 0), 5);
    }
    namedWindow("Control Points", WINDOW_NORMAL);
    imshow("Control Points", showImage);

    // 输出单位权中误差
    Matrix Qxx = (paramMatrix_A.transposition()*paramMatrix_A).reverse();
//    Qxx.print();

    for (int i=0; i<8; i++)
    {
        qDebug() << QString().sprintf("m_%d : %lf", i+1, sigma_0 * sqrt(Qxx[i][i]));
    }
    qDebug() << "-----";

    /* 使用 OpenCV 检校*/
    Mat sampleImage = currentImage.clone();
    // 图像坐标
    vector<Point2f> corners;
    vector<vector<Point2f>> cornersVect;
    // 世界坐标
    vector<Point3f> worldPoints;
    vector<vector<Point3f>> worldPointsVect;

    for (size_t i=0; i<points.size(); i++)
    {
        corners.push_back(Point2f(points[i].x, points[i].y));
        worldPoints.push_back(Point3f(points[i].X, points[i].Y, points[i].Z));
    }

    // 输出结果
    Mat cameraMatirx, distCoeffs;
    vector<Mat> rvecs,tvecs,rvecs2,tvecs2;

    cornersVect.push_back(corners);
    worldPointsVect.push_back(worldPoints);
    // 相机检校
    calibrateCamera(worldPointsVect,cornersVect,sampleImage.size(),cameraMatirx,distCoeffs,rvecs,tvecs);

    // 输出检校结果
    QString caliInfoStr = QString().sprintf("等效焦距(fx,fy)=(%.3lf,%.3lf)\n像主点(cx,cy)=(%.1lf,%.1lf)",
                                            cameraMatirx.at<double>(0, 0),
                                            cameraMatirx.at<double>(1, 1),
                                            cameraMatirx.at<double>(0, 2),
                                            cameraMatirx.at<double>(1, 2));
    caliInfoStr += QString("\n畸变系数(k1,k2,p1,p2[,k3[,k4,k5,k6[,s1,s2,s3,s4[,tx,ty]]]]) :\n");
    for (int i=0; i<distCoeffs.cols; i++)
    {
        caliInfoStr += QString().sprintf("%.8lf,", distCoeffs.at<double>(0, i));
    }
    caliInfoStr += QString().sprintf("\n外方位元素：\nφ=%.6lf\nω=%.6lf\nκ=%.6lf\nXs=%.3lf\nYs=%.3lf\nZs=%.3lf",
            rvecs[0].at<double>(0,0), rvecs[0].at<double>(1,0), rvecs[0].at<double>(2,0),
            tvecs[0].at<double>(0,0), tvecs[0].at<double>(1,0), tvecs[0].at<double>(2,0));
    showMessage(caliInfoStr, "检校信息");

}

void SampleData::on_orientation_element_initial_value_triggered()
{
    // 1. 计算内方位元素
    if (H.size() <= 1)
    {
        showMessage("请至少求解两张影像的DLT参数");
        return;
    }
    size_t dltNum, i;
    dltNum = H.size();
    // 构建线性矩阵
    Matrix L(dltNum, 2), c(dltNum, 1);
    for (i=0; i<dltNum; i++)
    {
        vector<double> h = H[i];
        double h1,h2,h3,h4,h5,h6,h7,h8;
        h1 = h[0]; h2 = h[1]; h3 = h[2]; h4 = h[3];
        h5 = h[4]; h6 = h[5]; h7 = h[6]; h8 = h[7];

        qDebug() << h1 << h2 << h3 << h4 << h5 << h6 << h7 << h8;

        L[i][0] = -h1*h7*h7*h8 - h1*h8*h8*h8 + h2*h7*h7*h7 + h2*h7*h8*h8;
        L[i][1] = -h4*h7*h7*h8 - h4*h8*h8*h8 + h5*h7*h7*h7 + h5*h7*h8*h8;
        c[i][0] = -h1*h1*h7*h8 - h1*h2*h8*h8 + h1*h2*h7*h7 + h2*h2*h7*h8
                - h4*h4*h7*h8 - h4*h5*h8*h8 + h4*h5*h7*h7 + h5*h5*h7*h8;

        qDebug() << L[i][0] << L[i][1] << c[i][0];
    }
    // 解算像主点
    Matrix principalPointX;
    principalPointX = (L.transposition()*L).reverse()*L.transposition()*c;
    double x0, y0;
    x0 = principalPointX[0][0]; y0 = principalPointX[1][0];
    qDebug() << "像主点(pixel): " << x0 << ", " << y0;

    qDebug() << "-----";

    // 计算误差
//    Matrix V = L*principalPointX - c;
//    V.print();

    // 2. 计算主距和外方位元素
    for (size_t i=0; i<H.size(); i++)
    {
        double h1,h2,h3,h4,h5,h6,h7,h8,f;
        vector<double> h = H[i];
        h1 = h[0]; h2 = h[1]; h3 = h[2]; h4 = h[3];
        h5 = h[4]; h6 = h[5]; h7 = h[6]; h8 = h[7];
        f = sqrt(abs(((h1-h7*x0)*(h2-h8*x0)+(h4-h7*y0)*(h5-h8*y0))/(h7*h8)));

        qDebug() << "主距：" << f;

        double kappa, omega, phi;
        // kappa
        kappa = atan((h2-h8*x0)/(h5-h8*x0));
        // omaga
        double b1, b2, b3;
        b3 = sqrt(1/(1 + pow(h2-h8*x0,2)/(f*f*h8*h8) + pow(h5-h8*y0,2)/(f*f*h8*h8)));
        omega = asin(b3);
        b1 = -(h2-h8*x0)*b3/(f*h8);
        b2 = -(h5-h8*y0)*b3/(f*h8);
        if (atan(b1/b2) != kappa)
        {
            omega = -asin(b3);
        }
        // phi
        phi = atan(1/((-(h1-h7*x0)/(f*h7))*b2 - (-(h4-h7*y0)/(f*h7))*b1));

        // 旋转矩阵R
        Matrix R(3, 3);
        R[0][0] = cos(phi)*cos(kappa) - sin(phi)*sin(omega)*sin(kappa);
        R[0][1] = cos(phi)*sin(kappa) - sin(phi)*sin(omega)*cos(kappa);
        R[0][2] = -sin(phi)*cos(omega);
        R[1][0] = cos(omega)*sin(kappa);
        R[1][1] = cos(omega)*cos(kappa);
        R[1][2] = -sin(omega);
        R[2][0] = sin(phi)*cos(kappa) + cos(phi)*sin(omega)*sin(kappa);
        R[2][1] = -sin(phi)*sin(kappa) + cos(phi)*sin(omega)*cos(kappa);
        R[2][2] = cos(phi)*cos(omega);

        // 取λ平均值
        double lambda = 0.0;
        lambda += f*R[0][0]/(h1-h7*x0);
        lambda += f*R[1][0]/(h2-h8*x0);
        lambda += f*R[0][1]/(h4-h7*y0);
        lambda += f*R[0][1]/(h5-h8*y0);
        lambda += -R[0][2]/h7;
        lambda += -R[1][2]/h8;
        lambda /= 6;

        Matrix L(3, 1);
        L[0][0] = lambda*(x0-h3)/f;
        L[1][0] = lambda*(y0-h6)/f;
        L[2][0] = lambda;

        Matrix X = (R.transposition()*R).reverse()*R.transposition()*L;

        qDebug() << "phi: " << phi;
        qDebug() << "omega: " << omega;
        qDebug() << "kappa: " << kappa;
        qDebug() << "Xs: " << X[0][0];
        qDebug() << "Ys: " << X[1][0];
        qDebug() << "Zs: " << X[2][0];


        // 计算新值
        double nH1,nH2,nH3,nH4,nH5,nH6,nH7,nH8;
        lambda = R[0][2]*X[0][0] + R[1][2]*X[1][0] + R[2][2]*X[2][0];
        nH1 = f*R[0][0]/lambda - R[0][2]*x0/lambda;
        nH2 = f*R[1][0]/lambda - R[1][2]*x0/lambda;
        nH3 = x0 - f*(R[0][0]*X[0][0]+R[1][0]*X[1][0]+R[2][0]*X[2][0])/lambda;
        nH4 = f*R[0][1]/lambda - R[0][2]*y0/lambda;
        nH5 = f*R[1][1]/lambda - R[1][2]*y0/lambda;
        nH6 = y0 - f*(R[0][1]*X[0][0]+R[1][1]*X[1][0]+R[2][1]*X[2][0])/lambda;
        nH7 = -R[0][2]/lambda;
        nH8 = -R[1][2]/lambda;

        qDebug() << "Old: " << h1 << h2 << h3 << h4 << h5 << h6 << h7 << h8;
        qDebug() << "New: " << nH1 << nH2 << nH3 << nH4 << nH5 << nH6 << nH7 << nH8;

        qDebug() << "------";
    }
}

int SampleData::showMessage(QString str, QString header)
{
    return QMessageBox::information(this,
                             header,
                             str);
}
