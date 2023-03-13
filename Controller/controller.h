#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../RemoteEvent/remoteevent.h"
#include "../NetworkApi/networkapi.h"
#include "../Socket/socket.h"

#include <QObject>
#include <QWidget>
#include <QMetaObject>
#include <QDateTime>

#include <memory>
#include <algorithm>
#include <string>

#ifdef Q_OS_WIN
    #include <opencv2/core.hpp>
    #include <opencv2/videoio.hpp>
#endif

class Socket;
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    Q_INVOKABLE void startRecording();
    Q_INVOKABLE void stopRecording();

    QPixmap picture;

public slots:
    void finish();

    void mousePressed(const QPointF &position, const Qt::MouseButton& btn);
    void mouseReleased(const QPointF &position, const Qt::MouseButton& btn);
    void mouseMoved(const QPointF &position, const Qt::MouseButton& btn);

    void keyPressed(const quint32 key);
    void keyReleased(const quint32 key);

    bool requestNewConnection(const QString &address);

signals:
    void connected();
    void disconnected();
    void needUpdate();

protected:
    void timerEvent(QTimerEvent *event);

private:
    inline void sendRemoteEvent(RemoteEvent::EventType type, const QPointF &position, const Qt::MouseButton& btn);
    inline void sendRemoteEvent(RemoteEvent::EventType type, const quint32 key);

    std::shared_ptr<Socket> serverSocket;

#ifdef Q_OS_WIN
    cv::VideoWriter video;
#endif
    int timerId = 0;

    const int fps = 30;
    const int msDelay = 1000/fps;
    const int port = 5555;
};

#endif // CONTROLLER_H
