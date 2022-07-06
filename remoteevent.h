#ifndef REMOTEEVENT_H
#define REMOTEEVENT_H


#include <QByteArray>
#include <QPointF>

class RemoteEvent
{
public:
    enum class EventType
    {
        NoType = 0,
        Pressed,
        Released,
        Moved,
        KeyPressed,
        KeyReleased
    };

    RemoteEvent() { }
    RemoteEvent(EventType type, const QPointF &position, const Qt::MouseButton& btn)
        : evType(type), evPosition(position), evButton(btn) { }

    EventType type() const { return evType; }
    void setType(EventType type) { evType = type; }

    QPointF position() const { return evPosition; }
    void setPosition(const QPointF &position) { evPosition = position; }

    Qt::MouseButton button() const { return evButton; }
    void setButton(const Qt::MouseButton &button) { evButton = button; }

    quint32 key() const { return evKey; }
    void setKey(const quint32 key) { evKey = key; }

    bool isEmpty() { return evType == EventType::NoType && evPosition.isNull(); }
    int size() { return sizeof(evType) + sizeof(evPosition) + sizeof(int); }

private:
    EventType evType = EventType::NoType;
    QPointF evPosition;
    Qt::MouseButton evButton;
    quint32 evKey;
};

#endif // REMOTEEVENT_H
