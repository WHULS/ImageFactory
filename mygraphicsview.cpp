#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView() :
    QGraphicsView ()
{

}

MyGraphicsView::~MyGraphicsView()
{

}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
        QPoint numPixels = event->pixelDelta();
        QPoint numDegrees = event->angleDelta() / 8;

        int step = 0;
        if (!numPixels.isNull()) {
            step = numPixels.y();
        } else if (!numDegrees.isNull()) {
            QPoint numSteps = numDegrees / 15;
            step = numSteps.y();
        }

        std::cout << step << std::endl;
    }

    event->accept();

    this->translate(1, 1);
}
