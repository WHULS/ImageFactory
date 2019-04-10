/********************************************************************************
** Form generated from reading UI file 'houghcircledlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOUGHCIRCLEDLG_H
#define UI_HOUGHCIRCLEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_HoughCircleDlg
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_7;
    QVBoxLayout *verticalLayout_2;
    QSlider *dp_slider;
    QSlider *min_dist_slider;
    QSlider *param1_slider;
    QSlider *param2_slider;
    QSlider *min_radius_slider;
    QSlider *max_radius_slider;

    void setupUi(QDialog *HoughCircleDlg)
    {
        if (HoughCircleDlg->objectName().isEmpty())
            HoughCircleDlg->setObjectName(QString::fromUtf8("HoughCircleDlg"));
        HoughCircleDlg->resize(360, 203);
        gridLayout = new QGridLayout(HoughCircleDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(HoughCircleDlg);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        label->setLayoutDirection(Qt::LeftToRight);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label);

        label_3 = new QLabel(HoughCircleDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_3);

        label_2 = new QLabel(HoughCircleDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_2);

        label_4 = new QLabel(HoughCircleDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_4);

        label_5 = new QLabel(HoughCircleDlg);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_5);

        label_7 = new QLabel(HoughCircleDlg);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);

        verticalLayout->addWidget(label_7);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        dp_slider = new QSlider(HoughCircleDlg);
        dp_slider->setObjectName(QString::fromUtf8("dp_slider"));
        dp_slider->setMinimum(1);
        dp_slider->setMaximum(5);
        dp_slider->setSingleStep(1);
        dp_slider->setValue(2);
        dp_slider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(dp_slider);

        min_dist_slider = new QSlider(HoughCircleDlg);
        min_dist_slider->setObjectName(QString::fromUtf8("min_dist_slider"));
        min_dist_slider->setMinimum(1);
        min_dist_slider->setMaximum(999);
        min_dist_slider->setValue(50);
        min_dist_slider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(min_dist_slider);

        param1_slider = new QSlider(HoughCircleDlg);
        param1_slider->setObjectName(QString::fromUtf8("param1_slider"));
        param1_slider->setMinimum(50);
        param1_slider->setMaximum(150);
        param1_slider->setValue(100);
        param1_slider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(param1_slider);

        param2_slider = new QSlider(HoughCircleDlg);
        param2_slider->setObjectName(QString::fromUtf8("param2_slider"));
        param2_slider->setMinimum(1);
        param2_slider->setMaximum(100);
        param2_slider->setValue(30);
        param2_slider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(param2_slider);

        min_radius_slider = new QSlider(HoughCircleDlg);
        min_radius_slider->setObjectName(QString::fromUtf8("min_radius_slider"));
        min_radius_slider->setMinimum(1);
        min_radius_slider->setMaximum(999);
        min_radius_slider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(min_radius_slider);

        max_radius_slider = new QSlider(HoughCircleDlg);
        max_radius_slider->setObjectName(QString::fromUtf8("max_radius_slider"));
        max_radius_slider->setMinimum(1);
        max_radius_slider->setMaximum(999);
        max_radius_slider->setValue(30);
        max_radius_slider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(max_radius_slider);


        horizontalLayout->addLayout(verticalLayout_2);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(HoughCircleDlg);

        QMetaObject::connectSlotsByName(HoughCircleDlg);
    } // setupUi

    void retranslateUi(QDialog *HoughCircleDlg)
    {
        HoughCircleDlg->setWindowTitle(QApplication::translate("HoughCircleDlg", "Hough\345\234\206\345\217\202\346\225\260", nullptr));
        label->setText(QApplication::translate("HoughCircleDlg", "dp", nullptr));
        label_3->setText(QApplication::translate("HoughCircleDlg", "minDist", nullptr));
        label_2->setText(QApplication::translate("HoughCircleDlg", "param1", nullptr));
        label_4->setText(QApplication::translate("HoughCircleDlg", "param2", nullptr));
        label_5->setText(QApplication::translate("HoughCircleDlg", "minRadius", nullptr));
        label_7->setText(QApplication::translate("HoughCircleDlg", "maxRadius", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HoughCircleDlg: public Ui_HoughCircleDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOUGHCIRCLEDLG_H
