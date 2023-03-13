#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

class ClickableLabel : public QLabel
{
    Q_OBJECT

signals:
    void clicked();
    void mousePressed( const QPointF&, const Qt::MouseButton&);
    void mouseReleased( const QPointF&, const Qt::MouseButton&);
    void mouseMoved(const QPointF&, const Qt::MouseButton&);

    void keyPressed(const quint32 key);
    void keyReleased(const quint32 key);
protected:
    void mousePressEvent ( QMouseEvent *event ) override;
    void mouseReleaseEvent( QMouseEvent *event ) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    //void wheelEvent(QWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
public:
    ClickableLabel(QWidget *parent=0): QLabel(parent){};
    virtual ~ClickableLabel() {};
};

#endif // CLICKABLELABEL_H
