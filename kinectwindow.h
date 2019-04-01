#ifndef KINECTWINDOW_H
#define KINECTWINDOW_H

#include <QWidget>

namespace Ui {
class KinectWindow;
}

class KinectWindow : public QWidget
{
    Q_OBJECT

public:
    explicit KinectWindow(QWidget *parent = nullptr);
    ~KinectWindow();
    bool init(); // 初始化Kinect

private:
    Ui::KinectWindow *ui;
};

#endif // KINECTWINDOW_H
