#include "MoravecDlg.h"
#include "ui_MoravecDlg.h"

MoravecDlg::MoravecDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MoravecDlg)
{
    ui->setupUi(this);
}

MoravecDlg::~MoravecDlg()
{
    delete ui;
}

void MoravecDlg::on_factorSize_valueChanged(int arg1)
{
    this->factorSize = arg1;
    emit ParamsChanged(this->factorSize, this->searchAreaSize);
}

void MoravecDlg::on_searchAreaSize_valueChanged(int arg1)
{
    this->factorSize = arg1;
    emit ParamsChanged(this->factorSize, this->searchAreaSize);
}
