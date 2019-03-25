#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QApplication>
#include <QWheelEvent>
#include <iostream>

class MyGraphicsView : public QGraphicsView
{
public:
    MyGraphicsView();
    ~MyGraphicsView();

private slots:
    void wheelEvent(QWheelEvent *);
};

#endif // MYGRAPHICSVIEW_H
