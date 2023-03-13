#include "controller.h"

#include <QGuiApplication>
#include <QHostAddress>
#include <QThread>

Controller::Controller(QObject *parent)
    : QObject(parent)
{
    serverSocket = std::make_shared<Socket>();
    QThread *thread = new QThread;
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(serverSocket.get(), &Socket::connected, this, &Controller::connected);
    connect(serverSocket.get(), &Socket::disconnected, this, &Controller::disconnected);
    connect(serverSocket.get(), &Socket::hasScreenData, this, [this](const QByteArray &screenData) {
        QPixmap pixmap;
        pixmap.loadFromData(screenData);
        picture = pixmap;
        emit needUpdate();
    });
    serverSocket->moveToThread(thread);
    thread->start();
}

void Controller::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
#ifdef Q_OS_WIN
    QImage image = picture.toImage();
    image = image.convertToFormat(QImage::Format_RGB888);
    QImage swapped = image.rgbSwapped();
    cv::Mat frame( swapped.height(), swapped.width(),
                                CV_8UC3,
                                const_cast<uchar*>(swapped.bits()),
                                static_cast<size_t>(swapped.bytesPerLine())
                                );
    video.write(frame);
#endif
}

void Controller::finish()
{
    QMetaObject::invokeMethod(serverSocket.get(), "abort");
}

void Controller::mousePressed(const QPointF &position, const Qt::MouseButton& btn)
{
    sendRemoteEvent(RemoteEvent::EventType::Pressed, position, btn);
}

void Controller::mouseReleased(const QPointF &position, const Qt::MouseButton& btn)
{
    sendRemoteEvent(RemoteEvent::EventType::Released, position, btn);
}

void Controller::mouseMoved(const QPointF &position, const Qt::MouseButton& btn)
{
    sendRemoteEvent(RemoteEvent::EventType::Moved, position, btn);
}

void Controller::keyPressed(const quint32 key)
{
    sendRemoteEvent(RemoteEvent::EventType::KeyPressed, key);
}

void Controller::keyReleased(const quint32 key)
{
    sendRemoteEvent(RemoteEvent::EventType::KeyReleased, key);
}

bool Controller::requestNewConnection(const QString &address)
{
    QHostAddress hostAddress(address);
    if (!hostAddress.isNull() && !NetworkApi::isLocalAddress(hostAddress)) {
        QMetaObject::invokeMethod(serverSocket.get(), "abort");
        QMetaObject::invokeMethod(serverSocket.get(), "connectHost", Q_ARG(QHostAddress, hostAddress), Q_ARG(quint16, port));
        return true;
    }
    return false;
}

void Controller::startRecording()
{
#ifdef Q_OS_WIN
    std::string filename = QDateTime::currentDateTime().toString(Qt::ISODate).toStdString() + ".avi";
    std::replace(filename.begin(), filename.end(), ':', '-');
    video = cv::VideoWriter(filename, cv::VideoWriter::fourcc('M','J','P','G'), fps, cv::Size(picture.width(), picture.height()));
    timerId = startTimer(std::chrono::milliseconds(msDelay));
#endif
}

void Controller::stopRecording()
{
#ifdef Q_OS_WIN
    killTimer(timerId);
    video.release();
#endif
}

void Controller::sendRemoteEvent(RemoteEvent::EventType type, const QPointF &position, const Qt::MouseButton& btn)
{
    RemoteEvent event(type, position, btn);
    QMetaObject::invokeMethod(serverSocket.get(), "writeToSocket", Q_ARG(RemoteEvent, event));
}

void Controller::sendRemoteEvent(RemoteEvent::EventType type, const quint32 key)
{
    RemoteEvent event;
    event.setType(type);
    event.setKey(key);
    QMetaObject::invokeMethod(serverSocket.get(), "writeToSocket", Q_ARG(RemoteEvent, event));
}
