#ifndef CONTROLPOINTDLG_H
#define CONTROLPOINTDLG_H

#include <QDialog>
#include <QTableWidgetItem>

#include "CaliImage.h"

namespace Ui {
class ControlPointDlg;
}

class ControlPointDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ControlPointDlg(QWidget *parent = nullptr);
    ~ControlPointDlg();
    void pushControlPoint(double X, double Y, double Z, int num);
    void pushControlPoint(CPoint cp);
    void clearControlPoint();
private slots:
    void on_controlPointList_itemClicked(QTableWidgetItem *item);

private:
    Ui::ControlPointDlg *ui;
    QStringList labels;
};

#endif // CONTROLPOINTDLG_H
