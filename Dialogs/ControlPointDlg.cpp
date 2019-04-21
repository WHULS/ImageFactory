#include "ControlPointDlg.h"
#include "ui_ControlPointDlg.h"

ControlPointDlg::ControlPointDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControlPointDlg)
{
    ui->setupUi(this);
}

ControlPointDlg::~ControlPointDlg()
{
    delete ui;
}

void ControlPointDlg::pushControlPoint(double X, double Y, double Z, int num)
{
    QTableWidget *table = ui->controlPointList;
    int row = table->rowCount();
    // 添加一行
    table->setRowCount(row + 1);

    labels << QString().sprintf("%d", row);
    table->setVerticalHeaderLabels(labels);

    QTableWidgetItem *itemN, *itemX, *itemY, *itemZ;
    itemN = new QTableWidgetItem();
    itemX = new QTableWidgetItem();
    itemY = new QTableWidgetItem();
    itemZ = new QTableWidgetItem();

    itemN->setText(QString().sprintf("%d", num));
    table->setItem(row, 0, itemN);
    itemX->setText(QString().sprintf("%lf", X));
    table->setItem(row, 1, itemX);
    itemY->setText(QString().sprintf("%lf", Y));
    table->setItem(row, 2, itemY);
    itemZ->setText(QString().sprintf("%lf", Z));
    table->setItem(row, 3, itemZ);
}

void ControlPointDlg::pushControlPoint(CPoint cp)
{
    double X, Y, Z;
    int num;
    X = cp.X;
    Y = cp.Y;
    Z = cp.Z;
    num = cp.num;
    pushControlPoint(X, Y, Z, num);
}

void ControlPointDlg::on_controlPointList_itemClicked(QTableWidgetItem *item)
{
    cout << item->text().toLocal8Bit().data() << endl;
}

void ControlPointDlg::clearControlPoint()
{
    QTableWidget *table = ui->controlPointList;
    // table->clear();
    table->setRowCount(0);
}
