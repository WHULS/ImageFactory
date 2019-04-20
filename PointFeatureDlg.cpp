#include "PointFeatureDlg.h"
#include "ui_PointFeatureDlg.h"

PointFeatureDlg::PointFeatureDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PointFeatureDlg)
{
    ui->setupUi(this);
}

PointFeatureDlg::~PointFeatureDlg()
{
    delete ui;
}

void PointFeatureDlg::on_horizontalSlider_valueChanged(int value)
{
    this->factorSize = value;
    emit ParamsChanged(this->factorSize, this->searchAreaSize);
}

void PointFeatureDlg::on_horizontalSlider_2_valueChanged(int value)
{
    this->searchAreaSize = value;
    emit ParamsChanged(this->factorSize, this->searchAreaSize);
}
