#include "controlled.h"

#include <QBuffer>
#include <QGuiApplication>
#include <QScreen>
#include <QTime>
#include <QThread>

Controlled::Controlled(QObject *parent)
    : QTcpServer (parent)
{
    listen(QHostAddress::Any, port);
}

Controlled::~Controlled()
{
}

void Controlled::finish()
{
    if (clientSocket)
    {
        QMetaObject::invokeMethod(clientSocket.get(), "abort");
    }
}

void Controlled::processEvent(const RemoteEvent &ev)
{
    QRectF screenRect = qApp->primaryScreen()->geometry();
    QPointF localPos(ev.position().x() * screenRect.width(),
                     ev.position().y() * screenRect.height());

    switch (ev.type())
    {
    case RemoteEvent::EventType::Pressed:
        SystemApi::mousePress(localPos, ev.button());
        break;
    case RemoteEvent::EventType::Released:
        SystemApi::mouseRelease(localPos, ev.button());
        break;
    case RemoteEvent::EventType::Moved:
        SystemApi::mouseMove(localPos, ev.button());
        break;
    case RemoteEvent::EventType::KeyPressed:
        SystemApi::keyPress(ev.key());
        break;
    case RemoteEvent::EventType::KeyReleased:
        SystemApi::keyRelease(ev.key());
        break;
    default:
        break;
    }
}

void Controlled::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (clientSocket)
    {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);

        QPixmap pixmap = SystemApi::grabScreen();
        pixmap.save(&buffer, "jpg", -1);

        BlockHeader header = { SCREEN_TYPE, qint32(buffer.size()) };
        DataBlock data = { header, buffer.data() };
        QMetaObject::invokeMethod(clientSocket.get(), "writeToSocket", Q_ARG(DataBlock, data));
    }
}

void Controlled::incomingConnection(qintptr socketDescriptor)
{
    if (!clientSocket) {
        clientSocket = std::make_shared<Socket>();
        QThread *thread = new QThread;
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        connect(clientSocket.get(), &Socket::disconnected, this, [this]() {
            finish();
            clientSocket.reset();
            killTimer(timerId);
            timerId = 0;
        });
        connect(clientSocket.get(), &Socket::hasEventData, this, [this](const RemoteEvent &event) {
            processEvent(event);
        });
        clientSocket->setSocketDescriptor(socketDescriptor);
        clientSocket->moveToThread(thread);
        thread->start();
        if (!timerId)
            timerId = startTimer(std::chrono::milliseconds(30));
    }
}
