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
}

SampleData::~SampleData()
{
    currentImage.release();
    delete ui;
}

void SampleData::dataListClicked(int row)
{
    switch (row) {
    case 0:
    {
        QTableView *dataInfo;
        dataInfo = ui->dataInfo; dataInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dataInfoModel = new QStandardItemModel(); dataInfo->setModel(dataInfoModel);

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


            namedWindow("Current Image", WINDOW_NORMAL);
            imshow("Current Image", currentImage);
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

        dataInfoModel = new QStandardItemModel(); dataInfo->setModel(dataInfoModel);

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

        dataInfoModel = new QStandardItemModel(); dataInfo->setModel(dataInfoModel);

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

                // 存入影像数组中
                caliImage.ControlPoints.push_back(cp);

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
            sd->caliImages.push_back(sd->caliImage);
            // 清空
            sd->caliImage.clear();
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
    
    caliImage.ImagePath = imagePath;

    Mat caliImage = imread(imagePath.toLocal8Bit().data(), 1);
    qDebug() << imagePath;
    if (!caliImage.empty() || caliImage.data)
    {
        namedWindow("Current Image", WINDOW_NORMAL);
        imshow("Current Image", caliImage);

        // 更新当前影像
        if (!currentImage.empty()) currentImage.release();
        currentImage = caliImage;

        // 事件监听
        setMouseCallback("Current Image", CPointMouseClick);

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

void SampleData::on_dataInfo_clicked(const QModelIndex &index)
{
    int row = index.row();
    // 当点击的是“当前影像”一栏的“控制点(row 3)”时
    if (row == 3 && (index.data().toString()==QString("CPoint") ||
                     index.siblingAtColumn(index.column()-1).data().toString()==QString("CPoint")))
    {
        Mat crossImage = currentImage.clone();
        Point center;
        for (size_t i=0; i<caliImage.ControlPoints.size(); i++)
        {
            // 画十字标，加上0.5四舍五入
            center = Point(caliImage.ControlPoints[i].x, caliImage.ControlPoints[i].y);
            drawCross(crossImage, center, 100, 3);
            // 画点号
            putText(crossImage, to_string(caliImage.ControlPoints[i].num), Point(center.x+10, center.y-10),
                    FONT_HERSHEY_SCRIPT_SIMPLEX, 5.0, Scalar(255, 0, 0), 5);
        }
        namedWindow("Control Points", WINDOW_NORMAL);
        imshow("Control Points", crossImage);
    }
}

// 保存信息到 xml 文件
void SampleData::on_save_calibration_info_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("保存"),
                                                    xmlDir,
                                                    tr("XML文件(*.xml)"));

    qDebug() << fileName;
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
    // ...
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
    QDomElement cImgs = doc.createElement("Calibration-Images");
    // ...
    root.appendChild(cImgs);

    // 文件保存
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
}
