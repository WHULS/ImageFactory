/********************************************************************************
** Form generated from reading UI file 'PointFeatureDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POINTFEATUREDLG_H
#define UI_POINTFEATUREDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>

QT_BEGIN_NAMESPACE

class Ui_PointFeatureDlg
{
public:
    QGridLayout *gridLayout;
    QSlider *horizontalSlider_2;
    QSlider *horizontalSlider;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *PointFeatureDlg)
    {
        if (PointFeatureDlg->objectName().isEmpty())
            PointFeatureDlg->setObjectName(QString::fromUtf8("PointFeatureDlg"));
        PointFeatureDlg->resize(400, 300);
        gridLayout = new QGridLayout(PointFeatureDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSlider_2 = new QSlider(PointFeatureDlg);
        horizontalSlider_2->setObjectName(QString::fromUtf8("horizontalSlider_2"));
        horizontalSlider_2->setMinimum(3);
        horizontalSlider_2->setMaximum(101);
        horizontalSlider_2->setSingleStep(2);
        horizontalSlider_2->setValue(9);
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider_2, 1, 1, 1, 1);

        horizontalSlider = new QSlider(PointFeatureDlg);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMinimum(3);
        horizontalSlider->setMaximum(101);
        horizontalSlider->setSingleStep(2);
        horizontalSlider->setValue(9);
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 0, 1, 1, 1);

        label = new QLabel(PointFeatureDlg);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(PointFeatureDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);


        retranslateUi(PointFeatureDlg);

        QMetaObject::connectSlotsByName(PointFeatureDlg);
    } // setupUi

    void retranslateUi(QDialog *PointFeatureDlg)
    {
        PointFeatureDlg->setWindowTitle(QApplication::translate("PointFeatureDlg", "Dialog", nullptr));
        label->setText(QApplication::translate("PointFeatureDlg", "\345\205\264\350\266\243\345\200\274\347\252\227\345\217\243", nullptr));
        label_2->setText(QApplication::translate("PointFeatureDlg", "\345\200\231\351\200\211\347\202\271\347\252\227\345\217\243", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PointFeatureDlg: public Ui_PointFeatureDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POINTFEATUREDLG_H
