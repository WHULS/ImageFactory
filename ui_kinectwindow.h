/********************************************************************************
** Form generated from reading UI file 'kinectwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KINECTWINDOW_H
#define UI_KINECTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KinectWindow
{
public:
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QWidget *KinectWindow)
    {
        if (KinectWindow->objectName().isEmpty())
            KinectWindow->setObjectName(QString::fromUtf8("KinectWindow"));
        KinectWindow->setEnabled(true);
        KinectWindow->resize(419, 346);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(KinectWindow->sizePolicy().hasHeightForWidth());
        KinectWindow->setSizePolicy(sizePolicy);
        label = new QLabel(KinectWindow);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(150, 150, 141, 16));
        pushButton = new QPushButton(KinectWindow);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(290, 270, 75, 23));

        retranslateUi(KinectWindow);
        QObject::connect(pushButton, SIGNAL(clicked()), KinectWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(KinectWindow);
    } // setupUi

    void retranslateUi(QWidget *KinectWindow)
    {
        KinectWindow->setWindowTitle(QApplication::translate("KinectWindow", "Kinect Window", nullptr));
        label->setText(QApplication::translate("KinectWindow", "\345\234\250\346\255\244\346\230\276\347\244\272Kinect\345\206\205\345\256\271", nullptr));
        pushButton->setText(QApplication::translate("KinectWindow", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KinectWindow: public Ui_KinectWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KINECTWINDOW_H
