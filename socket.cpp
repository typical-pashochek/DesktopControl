#include "remoteevent.h"
#include "socket.h"

Socket::Socket(QObject *parent)
    : QTcpSocket (parent)
{
    connect(this, &QTcpSocket::readyRead, this, [this]() {
        recvData += readAll();
        processRecvBlock();
    });
}

Socket::~Socket()
{

}

void Socket::abort()
{
    QTcpSocket::abort();
    recvData.clear();
    recvHeader.clear();
}

void Socket::writeToSocket(const DataBlock &block)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << block;
    write(data);
}

void Socket::writeToSocket(const RemoteEvent &event)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << event;
    BlockHeader header = { EVENT_TYPE, data.size() };
    DataBlock block = { header, data };
    out.device()->seek(0);
    out << block;
    write(data);
    flush();
}

void Socket::processRecvBlock()
{
    if (recvHeader.isEmpty() && recvData.size() > 0)
    {
        BlockHeader header;
        QDataStream in(&recvData, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_12);
        in >> header;

        if (header.isEmpty())
        {
            return;
        }

        recvHeader = header;
        recvData.remove(0, header.size());
    }

    if (recvData.size() < recvHeader.dataSize)
    {
        return;
    }

    DataBlock block;
    block.header = recvHeader;
    block.data = recvData.left(recvHeader.dataSize);
    recvData = recvData.mid(recvHeader.dataSize);
    recvHeader.clear();

    if (block.header.type == SCREEN_TYPE)
    {
        emit hasScreenData(block.data);
    }
    else if (block.header.type == EVENT_TYPE)
    {
        RemoteEvent event;
        QDataStream in(block.data);
        in.setVersion(QDataStream::Qt_5_12);
        in >> event;
        emit hasEventData(event);
    }

    if (recvData.size() > 0)
    {
        processRecvBlock();
    }
}
