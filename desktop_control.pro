QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ClickableLabel/clickablelabel.cpp \
    Controlled/controlled.cpp \
    Controller/controller.cpp \
    NetworkApi/networkapi.cpp \
    Protocol/protocol.cpp \
    Socket/socket.cpp \
    SystemApi/systemapi.cpp \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    ClickableLabel/clickablelabel.h \
    Controlled/controlled.h \
    Controller/controller.h \
    DXGIManager/dxgimanager.h \
    NetworkApi/networkapi.h \
    Protocol/protocol.h \
    RemoteEvent/remoteevent.h \
    Socket/socket.h \
    SystemApi/systemapi.h\
    mainwindow.h

FORMS += \
    mainwindow.ui

win32{
    QT += winextras
    LIBS += -lGdi32
    SOURCES += DXGIManager/dxgimanager.cpp
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
