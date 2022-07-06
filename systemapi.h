#ifndef SYSTEMAPI_H
#define SYSTEMAPI_H

#include <QPixmap>

class SystemApi
{
public:
    static void mousePress(const QPointF &pos, const Qt::MouseButton btn);
    static void mouseRelease(const QPointF &pos, const Qt::MouseButton btn);
    static void mouseMove(const QPointF &pos, const Qt::MouseButton btn);

    static void keyPress(const quint32 key);
    static void keyRelease(const quint32 key);

    static QPixmap grabScreen();
    static QPixmap grabCursor();

private:
    static inline const std::map<quint32, quint32> vKeyToKeysym = {
            {0x08, 0xFF08}, //backspace
            {0x09, 0xFF09}, //tab
            {0x0D, 0xFF0D}, //return
            {0x10, 0xFFE1}, //shift
            {0x11, 0xFFE3}, //ctrl
            {0x12, 0xFFE9}, //alt
            {0x14, 0xFFE5}, //caps lock
            {0x1B, 0xFF1B}, //esc
            {0x21, 0xFF55}, //page up
            {0x22, 0xFF56}, //page down
            {0x23, 0xFF57}, //end
            {0x24, 0xFF50}, //home
            {0x25, 0xFF51}, //left arrow
            {0x26, 0xFF52}, //up arrow
            {0x27, 0xFF53}, //right arrow
            {0x28, 0xFF54}, //down arrow
            {0x2E, 0xFFFF}, //del
            {0xBA, 0x3B},   //:;
            {0xBB, 0x3D},   //+=
            {0xBC, 0x2C},   //<,
            {0xBD, 0x2D},   //_-
            {0xBE, 0x2E},   //>.
            {0xBF, 0x2F},   //?/
            {0xC0, 0x60},   //~`
            {0xDB, 0x5B},   //{[
            {0xDC, 0x5C},   //\|
            {0xDD, 0x5D},   //}]
            {0xDE, 0x27}    //"'
        };
};

#endif // SYSTEMAPI_H
