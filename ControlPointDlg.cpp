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

    labels << QString(row);
    table->setVerticalHeaderLabels(labels);

    QTableWidgetItem *itemX, *itemY, *itemZ, *itemN;
    itemX = new QTableWidgetItem();
    itemY = new QTableWidgetItem();
    itemZ = new QTableWidgetItem();
    itemN = new QTableWidgetItem();

    itemX->setText(QString("%lf").arg(X));
    table->setItem(row, 0, itemX);
    itemX->setText(QString("%lf").arg(Y));
    table->setItem(row, 1, itemY);
    itemX->setText(QString("%lf").arg(Z));
    table->setItem(row, 2, itemZ);
    itemX->setText(QString("%lf").arg(num));
    table->setItem(row, 3, itemN);
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
