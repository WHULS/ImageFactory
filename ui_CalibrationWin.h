/********************************************************************************
** Form generated from reading UI file 'CalibrationWin.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATIONWIN_H
#define UI_CALIBRATIONWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CalibrationWin
{
public:
    QAction *open_image;
    QAction *calibration;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CalibrationWin)
    {
        if (CalibrationWin->objectName().isEmpty())
            CalibrationWin->setObjectName(QString::fromUtf8("CalibrationWin"));
        CalibrationWin->resize(312, 222);
        open_image = new QAction(CalibrationWin);
        open_image->setObjectName(QString::fromUtf8("open_image"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/image.png"), QSize(), QIcon::Normal, QIcon::Off);
        open_image->setIcon(icon);
        calibration = new QAction(CalibrationWin);
        calibration->setObjectName(QString::fromUtf8("calibration"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/images/tool.png"), QSize(), QIcon::Normal, QIcon::Off);
        calibration->setIcon(icon1);
        centralwidget = new QWidget(CalibrationWin);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        CalibrationWin->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CalibrationWin);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 312, 23));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        CalibrationWin->setMenuBar(menubar);
        statusbar = new QStatusBar(CalibrationWin);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        CalibrationWin->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menu->addAction(open_image);
        menu->addAction(calibration);

        retranslateUi(CalibrationWin);

        QMetaObject::connectSlotsByName(CalibrationWin);
    } // setupUi

    void retranslateUi(QMainWindow *CalibrationWin)
    {
        CalibrationWin->setWindowTitle(QApplication::translate("CalibrationWin", "MainWindow", nullptr));
        open_image->setText(QApplication::translate("CalibrationWin", "\346\211\223\345\274\200\345\233\276\345\203\217 (&O)", nullptr));
        calibration->setText(QApplication::translate("CalibrationWin", "\346\243\200\346\240\241 (&C)", nullptr));
        menu->setTitle(QApplication::translate("CalibrationWin", "\345\233\276\345\203\217 (&I)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalibrationWin: public Ui_CalibrationWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATIONWIN_H
