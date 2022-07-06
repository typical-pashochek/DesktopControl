#include "systemapi.h"

#include <QCursor>
#include <QDebug>
#include <QGuiApplication>
#include <QPainter>
#include <QWindow>
#include <QScreen>

#ifdef Q_OS_WIN
    # include <windows.h>
    # include <QtWin>
    # if defined (_WIN32_WINNT_WIN10)
    #   include "dxgimanager.h"
    #  endif
#elif defined(Q_OS_ANDROID)
    #include <GLES3/gl3ext.h>
    #include <GLES3/gl3.h>
    #include <QtAndroid>
    #include <QAndroidJniObject>
    #include <QAndroidJniEnvironment>
#elif defined(Q_OS_LINUX)
    #include <unistd.h>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/extensions/XTest.h>
#endif

void SystemApi::mousePress(const QPointF &pos, const Qt::MouseButton btn)
{
#ifdef Q_OS_WIN
    SetCursorPos(int(pos.x()), int(pos.y()));
    INPUT input;
    memset(&input, 0, sizeof(INPUT));
    input.type = INPUT_MOUSE;

    if (btn & Qt::LeftButton)
    {
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    }
    else if (btn & Qt::MiddleButton){
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
    }
    else if (btn & Qt::RightButton){
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    }
    SendInput(1, &input, sizeof(INPUT));
#elif defined(Q_OS_ANDROID)
    system("input tap 10 10");
#elif defined(Q_OS_LINUX)
    Display *display = XOpenDisplay (NULL);
    Window root = DefaultRootWindow(display);
    XWarpPointer(display, None, root, 0, 0, 0, 0, pos.x(), pos.y());
    if (btn & Qt::LeftButton)
    {
        XTestFakeButtonEvent (display, 1, True,  CurrentTime);
    }
    else if (btn & Qt::RightButton){
        XTestFakeButtonEvent (display, 3, True,  CurrentTime);
    }

    XFlush (display);
    XCloseDisplay (display);
#endif
}

void SystemApi::mouseRelease(const QPointF &pos, const Qt::MouseButton btn)
{
#ifdef Q_OS_WIN
    SetCursorPos(int(pos.x()), int(pos.y()));
    INPUT input;
    memset (&input, 0, sizeof(INPUT));
    input.type = INPUT_MOUSE;

    if (btn & Qt::LeftButton)
    {
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    }
    else if (btn & Qt::MiddleButton){
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
    }
    else if (btn & Qt::RightButton){
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    }
    SendInput(1, &input, sizeof(INPUT));
#elif defined(Q_OS_ANDROID)

#elif defined(Q_OS_LINUX)
    Display *display = XOpenDisplay (NULL);
    Window root = DefaultRootWindow(display);
    XWarpPointer(display, None, root, 0, 0, 0, 0, pos.x(), pos.y());
    if (btn & Qt::LeftButton)
    {
        XTestFakeButtonEvent (display, 1, False,  CurrentTime);
    }
    else if (btn & Qt::RightButton){
        XTestFakeButtonEvent (display, 3, False,  CurrentTime);
    }
    XFlush (display);
    XCloseDisplay (display);
#endif
}

void SystemApi::mouseMove(const QPointF &pos, const Qt::MouseButton btn)
{
#ifdef Q_OS_WIN
    SetCursorPos(int(pos.x()), int(pos.y()));
    INPUT input;
    memset (&input, 0, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(INPUT));
#elif defined(Q_OS_ANDROID)

#elif defined(Q_OS_LINUX)
    Display *display = XOpenDisplay (NULL);
    Window root = DefaultRootWindow(display);
    XWarpPointer(display, None, root, 0, 0, 0, 0, pos.x(), pos.y());
    XFlush (display);
    XCloseDisplay (display);
#endif
}

void SystemApi::keyPress(const quint32 key)
{
#ifdef Q_OS_WIN
    INPUT input;
    memset (&input, 0, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    SendInput(1, &input, sizeof(INPUT));
#elif defined(Q_OS_ANDROID)

#elif defined(Q_OS_LINUX)
    Display *display = XOpenDisplay (NULL);
    Window root = DefaultRootWindow(display);
    if (vKeyToKeysym.find(key) != vKeyToKeysym.end())
    {
        auto keysym = vKeyToKeysym.find(key)->second;
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, keysym), True, CurrentTime);
    }
    else
    {
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, key), True, CurrentTime);
    }
    XFlush(display);
    XCloseDisplay (display);
#endif
}

void SystemApi::keyRelease(const quint32 key)
{
#ifdef Q_OS_WIN
    INPUT input;
    memset (&input, 0, sizeof(INPUT));
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
#elif defined(Q_OS_ANDROID)

#elif defined(Q_OS_LINUX)
    Display *display = XOpenDisplay (NULL);
    Window root = DefaultRootWindow(display);
    if (vKeyToKeysym.find(key) != vKeyToKeysym.end())
    {
        auto keysym = vKeyToKeysym.find(key)->second;
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, keysym), False, CurrentTime);
    }
    else
    {
        XTestFakeKeyEvent(display, XKeysymToKeycode(display, key), False, CurrentTime);
    }
    XFlush(display);
    XCloseDisplay (display);
#endif
}


QPixmap SystemApi::grabScreen()
{
    QPixmap screen;
#if defined (Q_OS_WIN) && defined (_WIN32_WINNT_WIN10)
    static DxgiManager dxgiManager;
    static bool dxgiInit = false;
    if (!dxgiInit) {
        if (!dxgiManager.init()) {
            qDebug() << "DXGI Init Error :" << dxgiManager.lastError();
        } else {
            dxgiInit = true;
            screen = dxgiManager.grabScreen();
        }
    } else {
         screen = dxgiManager.grabScreen();
    }
#elif defined (Q_OS_ANDROID)
    screen = QGuiApplication::focusWindow()->screen()->grabWindow(0);
#else
    screen = QGuiApplication::primaryScreen()->grabWindow(0);
#endif
    if (!screen.isNull()) {
        QPainter painter(&screen);
        painter.drawPixmap(QCursor::pos(), grabCursor());
    }

    return screen;
}

QPixmap SystemApi::grabCursor()
{
    QPixmap cursorPixmap;
#ifdef Q_OS_WIN
    // Get Cursor Size
    int cursorWidth = GetSystemMetrics(SM_CXCURSOR);
    int cursorHeight = GetSystemMetrics(SM_CYCURSOR);

    // Get your device contexts.
    HDC hdcScreen = GetDC(nullptr);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);

    // Create the bitmap to use as a canvas.
    HBITMAP hbmCanvas = CreateCompatibleBitmap(hdcScreen, cursorWidth, cursorHeight);

    // Select the bitmap into the device context.
    HGDIOBJ hbmOld = SelectObject(hdcMem, hbmCanvas);

    // Get information about the global cursor.
    CURSORINFO ci;
    ci.cbSize = sizeof(ci);
    GetCursorInfo(&ci);

    // Draw the cursor into the canvas.
    DrawIcon(hdcMem, 0, 0, ci.hCursor);

    // Convert to QPixmap
    cursorPixmap = QtWin::fromHBITMAP(hbmCanvas, QtWin::HBitmapAlpha);

    // Clean up after yourself.
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmCanvas);
    DeleteDC(hdcMem);
    ReleaseDC(nullptr, hdcScreen);
#endif
    return cursorPixmap;
}
