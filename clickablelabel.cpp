#include "clickablelabel.h"

void ClickableLabel::mousePressEvent ( QMouseEvent * event )
{
    this->setFocus();
    QPointF point = event->pos();
    point.setX(point.x() / this->width());
    point.setY(point.y() / this->height());
    emit this->mousePressed(point, event->button());
};

void ClickableLabel::mouseReleaseEvent( QMouseEvent* event )
{
    QPointF point = event->pos();
    point.setX(point.x() / this->width());
    point.setY(point.y() / this->height());
    emit this->mouseReleased(point, event->button());
};

void ClickableLabel::mouseMoveEvent(QMouseEvent *event) {
    QPointF point = event->pos();
    point.setX(point.x() / this->width());
    point.setY(point.y() / this->height());
    emit this->mouseMoved(point, event->button());
}

void ClickableLabel::keyPressEvent(QKeyEvent *event)
{    
    emit this->keyPressed(event->nativeVirtualKey());
}

void ClickableLabel::keyReleaseEvent(QKeyEvent *event)
{
    emit this->keyReleased(event->nativeVirtualKey());
}
