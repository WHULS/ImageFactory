#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QApplication>
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>

using namespace std;

class MyGraphicsView : public QGraphicsView
{
public:
    MyGraphicsView();
    ~MyGraphicsView();

private slots:
    void mousePressEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
};

#endif // MYGRAPHICSVIEW_H
