#include "myslider.h"


MySlider::MySlider(Qt::Orientation orientation, QWidget *parent) : QSlider (orientation, parent)
{
    // 连接信号和槽
//    connect(this, SIGNAL(valueChanged(int)), this, SLOT(detectValueChange(int)));
}

MySlider::MySlider(QWidget *parent) : QSlider(parent)
{
    // 连接信号和槽
//    connect(this, SIGNAL(valueChanged(int)), this, SLOT(detectValueChange(int)));
}

MySlider::~MySlider()
{

}

//void MySlider::detectValueChange(int value)
//{
//    std::cout << value << std::endl;
//}
