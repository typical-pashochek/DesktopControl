#include "protocol.h"
#include "remoteevent.h"

QDataStream& operator>>(QDataStream &in, BlockHeader &header)
{
    in >> header.type
       >> header.dataSize;

    return in;
}

QDataStream& operator<<(QDataStream &out, const BlockHeader &header)
{
    out << header.type
        << header.dataSize;

    return out;
}

QDataStream& operator>>(QDataStream &in, DataBlock &block)
{
    in >> block.header
       >> block.data;

    return in;
}

QDataStream& operator<<(QDataStream &out, const DataBlock &block)
{
    out << block.header
        << block.data;

    return out;
}

QDataStream &operator>>(QDataStream &in, RemoteEvent &block)
{
    qint32 type;
    Qt::MouseButton button;
    QPointF position;
    quint32 key;
    in >> type >> position >> button >> key;
    block.setType(RemoteEvent::EventType(type));
    block.setPosition(position);
    block.setButton(button);
    block.setKey(key);

    return in;
}

QDataStream &operator<<(QDataStream &out, const RemoteEvent &block)
{
    out << qint32(block.type())
        << block.position()
        << block.button()
        << block.key();

    return out;
}
