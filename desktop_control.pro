QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clickablelabel.cpp \
    controlled.cpp \
    controller.cpp \
    main.cpp \
    mainwindow.cpp \
    networkapi.cpp \
    protocol.cpp \
    socket.cpp \
    systemapi.cpp

HEADERS += \
    clickablelabel.h \
    controlled.h \
    controller.h \
    dxgimanager.h \
    mainwindow.h \
    networkapi.h \
    protocol.h \
    remoteevent.h \
    socket.h \
    systemapi.h

FORMS += \
    mainwindow.ui

win32{
    QT += winextras
    LIBS += -lGdi32
    SOURCES += dxgimanager.cpp
    LIBS += -lD3D11 -lDXGI
    INCLUDEPATH += D:\open\opencv\build\include
    LIBS += D:\open\build\bin\libopencv_core460.dll
    LIBS += D:\open\build\bin\libopencv_highgui460.dll
    LIBS += D:\open\build\bin\libopencv_videoio460.dll
}
linux:!android{
    LIBS += -lX11 -lXtst
}
android{
    QT += androidextras
}

DEFINES += QT_DEPRECATED_WARNINGS

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
