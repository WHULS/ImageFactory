#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QSlider>
#include <iostream>
#include <QMouseEvent>

class MySlider : public QSlider
{
    // 一定要加这个，不然slots不管用（No such slot QSlider::detectValueChange(int)）
    /*
     * 由于要在构造函数中使用connect函数，如果不声明这个，就会提示找不到slot，
     * 因为Q_OBJECT只在QSlider中存在，connect函数只会在QSlider函数中寻找slot
     * 如何使Q_OBJECT生效？
     * 1. 首先清空生成文件夹；
     * 2. 执行qmake
     * 3. 启动运行（Ctrl + R）
     */
    Q_OBJECT

public:
    MySlider(Qt::Orientation, QWidget *parent = nullptr);
    MySlider(QWidget *parent = nullptr);
    ~MySlider();

public slots:
//    void detectValueChange(int);
};

#endif // MYSLIDER_H
