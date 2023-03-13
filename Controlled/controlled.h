#ifndef CONTROLLED_H
#define CONTROLLED_H

#include "../Protocol/protocol.h"
#include "../RemoteEvent/remoteevent.h"
#include "../SystemApi/systemapi.h"
#include "../Socket/socket.h"

#include <QObject>
#include <QTcpServer>
#include <QTimer>

#include <memory>

class Socket;
class RemoteEvent;
class Controlled : public QTcpServer
{
    Q_OBJECT

public:
    explicit Controlled(QObject *parent = nullptr);
    ~Controlled();

    Q_INVOKABLE void finish();

signals:
    void connected();
    void disconnected();

public slots:
    void processEvent(const RemoteEvent &ev);

protected:
    void timerEvent(QTimerEvent *event);
    void incomingConnection(qintptr socketDescriptor);

private:
    std::shared_ptr<Socket> clientSocket;
    std::shared_ptr<QThread> clientThread;
    int timerId = 0;
    const int port = 5555;
};

#endif // CONTROLLED_H
