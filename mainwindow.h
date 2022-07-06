#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <iostream>
#include <stdio.h>
#include <ctime>
//#include <unistd.h>
#include <memory>

#include <QMainWindow>
#include <QStyleFactory>
#include <QFileDialog>
#include <QLabel>
#include <QComboBox>

#include <QUdpSocket>

#include <QImage>
#include "controller.h"
#include "controlled.h"
#include "clickablelabel.h"
#include "networkapi.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QHostAddress ip = QHostAddress("127.0.0.1");
    int port = 5555;

private slots:
    void connectPushButtonClicked();
    void waitPushButtonClicked();
    void stopSharingPushButtonClicked();
    void disconnectPushButtonClicked();
    void recordingPushButtonClicked();
    void setPixmap();
private:
    Ui::MainWindow *ui;

    std::shared_ptr<Controlled> controlled;
    std::shared_ptr<Controller> controller;

    std::shared_ptr<ClickableLabel> pictureLabel;
    std::shared_ptr<QPushButton> disconnectPushButton;
    std::shared_ptr<QPushButton> recordingPushButton;
    //std::shared_ptr<QThread> controlledThread;

    bool isRecording = false;

};
#endif // MAINWINDOW_H
