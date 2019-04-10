#ifndef CONTROLPOINTDLG_H
#define CONTROLPOINTDLG_H

#include <QDialog>

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

private:
    Ui::ControlPointDlg *ui;
    QStringList labels;
};

#endif // CONTROLPOINTDLG_H
