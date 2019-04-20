#ifndef POINTFEATUREDLG_H
#define POINTFEATUREDLG_H

#include <QDialog>

namespace Ui {
class PointFeatureDlg;
}

class PointFeatureDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PointFeatureDlg(QWidget *parent = nullptr);
    ~PointFeatureDlg();

    int factorSize = 9;
    int searchAreaSize = 9;

signals:
    void ParamsChanged(int factorSize, int searchAreaSize);

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

private:
    Ui::PointFeatureDlg *ui;
};

#endif // POINTFEATUREDLG_H
