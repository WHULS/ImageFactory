/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *image_open;
    QAction *edge_canny;
    QAction *clear_image;
    QAction *show_srcImage;
    QAction *fit_to_image;
    QAction *edge_laplacian;
    QAction *edge_log;
    QAction *edge_sobel;
    QAction *edge_roberts;
    QAction *edge_prewitt;
    QAction *show_edge;
    QAction *show_blur;
    QAction *show_gray;
    QAction *edge_dog;
    QAction *calibration;
    QAction *read_control_point;
    QWidget *centralWidget;
    QToolBar *toolBar;
    QMenuBar *menuBar;
    QMenu *menu_F;
    QMenu *menu;
    QMenu *menu_Y;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(292, 251);
        image_open = new QAction(MainWindow);
        image_open->setObjectName(QString::fromUtf8("image_open"));
        image_open->setCheckable(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/image.png"), QSize(), QIcon::Normal, QIcon::Off);
        image_open->setIcon(icon);
        edge_canny = new QAction(MainWindow);
        edge_canny->setObjectName(QString::fromUtf8("edge_canny"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/images/Canny.png"), QSize(), QIcon::Normal, QIcon::Off);
        edge_canny->setIcon(icon1);
        clear_image = new QAction(MainWindow);
        clear_image->setObjectName(QString::fromUtf8("clear_image"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/images/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        clear_image->setIcon(icon2);
        show_srcImage = new QAction(MainWindow);
        show_srcImage->setObjectName(QString::fromUtf8("show_srcImage"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/images/geren.png"), QSize(), QIcon::Normal, QIcon::Off);
        show_srcImage->setIcon(icon3);
        fit_to_image = new QAction(MainWindow);
        fit_to_image->setObjectName(QString::fromUtf8("fit_to_image"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/images/resize.png"), QSize(), QIcon::Normal, QIcon::Off);
        fit_to_image->setIcon(icon4);
        edge_laplacian = new QAction(MainWindow);
        edge_laplacian->setObjectName(QString::fromUtf8("edge_laplacian"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/images/Laplacian.png"), QSize(), QIcon::Normal, QIcon::Off);
        edge_laplacian->setIcon(icon5);
        edge_log = new QAction(MainWindow);
        edge_log->setObjectName(QString::fromUtf8("edge_log"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/images/Log.png"), QSize(), QIcon::Normal, QIcon::Off);
        edge_log->setIcon(icon6);
        edge_sobel = new QAction(MainWindow);
        edge_sobel->setObjectName(QString::fromUtf8("edge_sobel"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/images/Sobel.png"), QSize(), QIcon::Normal, QIcon::Off);
        edge_sobel->setIcon(icon7);
        edge_roberts = new QAction(MainWindow);
        edge_roberts->setObjectName(QString::fromUtf8("edge_roberts"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/images/Robert.png"), QSize(), QIcon::Normal, QIcon::Off);
        edge_roberts->setIcon(icon8);
        edge_prewitt = new QAction(MainWindow);
        edge_prewitt->setObjectName(QString::fromUtf8("edge_prewitt"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/images/Prewitt.png"), QSize(), QIcon::Normal, QIcon::Off);
        edge_prewitt->setIcon(icon9);
        show_edge = new QAction(MainWindow);
        show_edge->setObjectName(QString::fromUtf8("show_edge"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/images/datadig.png"), QSize(), QIcon::Normal, QIcon::Off);
        show_edge->setIcon(icon10);
        show_blur = new QAction(MainWindow);
        show_blur->setObjectName(QString::fromUtf8("show_blur"));
        show_blur->setIcon(icon10);
        show_gray = new QAction(MainWindow);
        show_gray->setObjectName(QString::fromUtf8("show_gray"));
        show_gray->setIcon(icon10);
        edge_dog = new QAction(MainWindow);
        edge_dog->setObjectName(QString::fromUtf8("edge_dog"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/images/images/DOG.png"), QSize(), QIcon::Normal, QIcon::Off);
        edge_dog->setIcon(icon11);
        calibration = new QAction(MainWindow);
        calibration->setObjectName(QString::fromUtf8("calibration"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/icons/camico2.ico"), QSize(), QIcon::Normal, QIcon::Off);
        calibration->setIcon(icon12);
        read_control_point = new QAction(MainWindow);
        read_control_point->setObjectName(QString::fromUtf8("read_control_point"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/images/images/dingwei.png"), QSize(), QIcon::Normal, QIcon::Off);
        read_control_point->setIcon(icon13);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 292, 23));
        menu_F = new QMenu(menuBar);
        menu_F->setObjectName(QString::fromUtf8("menu_F"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_Y = new QMenu(menuBar);
        menu_Y->setObjectName(QString::fromUtf8("menu_Y"));
        MainWindow->setMenuBar(menuBar);

        toolBar->addAction(image_open);
        toolBar->addAction(fit_to_image);
        toolBar->addAction(show_srcImage);
        toolBar->addSeparator();
        toolBar->addAction(show_edge);
        toolBar->addAction(show_blur);
        toolBar->addAction(show_gray);
        toolBar->addSeparator();
        toolBar->addAction(clear_image);
        toolBar->addSeparator();
        menuBar->addAction(menu_F->menuAction());
        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_Y->menuAction());
        menu_F->addAction(image_open);
        menu->addAction(edge_canny);
        menu->addAction(edge_laplacian);
        menu->addAction(edge_log);
        menu->addAction(edge_sobel);
        menu->addAction(edge_roberts);
        menu->addAction(edge_prewitt);
        menu->addAction(edge_dog);
        menu_Y->addAction(read_control_point);
        menu_Y->addAction(calibration);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "ImageFactory", nullptr));
        image_open->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200(&O)", nullptr));
#ifndef QT_NO_STATUSTIP
        image_open->setStatusTip(QApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\200\345\271\205\345\233\276\345\203\217", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_SHORTCUT
        image_open->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        edge_canny->setText(QApplication::translate("MainWindow", "Canny (&C)", nullptr));
        clear_image->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272(&D)", nullptr));
#ifndef QT_NO_STATUSTIP
        clear_image->setStatusTip(QApplication::translate("MainWindow", "\346\270\205\347\251\272\347\273\230\345\233\276\345\214\272", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_SHORTCUT
        clear_image->setShortcut(QApplication::translate("MainWindow", "Ctrl+D", nullptr));
#endif // QT_NO_SHORTCUT
        show_srcImage->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\345\216\237\345\233\276(&R)", nullptr));
#ifndef QT_NO_SHORTCUT
        show_srcImage->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_NO_SHORTCUT
        fit_to_image->setText(QApplication::translate("MainWindow", "\351\200\202\345\272\224\345\233\276\345\203\217(&I)", nullptr));
#ifndef QT_NO_SHORTCUT
        fit_to_image->setShortcut(QApplication::translate("MainWindow", "Shift+F", nullptr));
#endif // QT_NO_SHORTCUT
        edge_laplacian->setText(QApplication::translate("MainWindow", "Laplacian (&L)", nullptr));
        edge_log->setText(QApplication::translate("MainWindow", "LOG (&G)", nullptr));
        edge_sobel->setText(QApplication::translate("MainWindow", "Sobel (&S)", nullptr));
        edge_roberts->setText(QApplication::translate("MainWindow", "Roberts (&R)", nullptr));
        edge_prewitt->setText(QApplication::translate("MainWindow", "Prewitt (&P)", nullptr));
        show_edge->setText(QApplication::translate("MainWindow", "\350\276\271\347\274\230\345\233\276 (&E)", nullptr));
#ifndef QT_NO_SHORTCUT
        show_edge->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_NO_SHORTCUT
        show_blur->setText(QApplication::translate("MainWindow", "\346\250\241\347\263\212\345\233\276 (&B)", nullptr));
        show_gray->setText(QApplication::translate("MainWindow", "\347\201\260\345\272\246\345\233\276 (&G)", nullptr));
        edge_dog->setText(QApplication::translate("MainWindow", "DOG (&D)", nullptr));
        calibration->setText(QApplication::translate("MainWindow", "\346\221\204\345\275\261\346\234\272\345\256\232\346\240\207(&B)", nullptr));
#ifndef QT_NO_TOOLTIP
        calibration->setToolTip(QApplication::translate("MainWindow", "\346\221\204\345\275\261\346\234\272\345\256\232\346\240\207(B)", nullptr));
#endif // QT_NO_TOOLTIP
        read_control_point->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226\346\216\247\345\210\266\347\202\271\346\226\207\344\273\266(&O)", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
        menu_F->setTitle(QApplication::translate("MainWindow", "\345\233\276\345\203\217(&F)", nullptr));
        menu->setTitle(QApplication::translate("MainWindow", "\347\211\271\345\276\201\346\217\220\345\217\226(&A)", nullptr));
        menu_Y->setTitle(QApplication::translate("MainWindow", "\347\202\271\344\272\221\346\217\220\345\217\226 (&Y)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
