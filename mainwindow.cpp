#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScreen>
#include <QDesktopWidget>
#include <QThread>

#include <QNetworkInterface>
#include <QMessageBox>
#include <QFileDialog>
#include <QBuffer>

#include <QImageWriter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->move(382, 134);
    ui->setupUi(this);

    ui->myIPLabel->setText("Ваш ip: " + NetworkApi::getLocalIpAddress());

    connect(ui->connectPushButton, SIGNAL(clicked()), this, SLOT(connectPushButtonClicked()));
    connect(ui->waitPushButton, SIGNAL(clicked()), this, SLOT(waitPushButtonClicked()));
    connect(ui->stopSharingPushButton, SIGNAL(clicked()), this, SLOT(stopSharingPushButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectPushButtonClicked()
{
    controller = std::make_shared<Controller>();
    if (!controller->requestNewConnection(ui->ipTextEdit->toPlainText()))
    {
        return;
    }

    ui->stackedWidget->setCurrentIndex(1);
    MainWindow::showMaximized();

    pictureLabel = std::make_shared<ClickableLabel>(this);
    pictureLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    pictureLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    pictureLabel->setGeometry(10,10, 1280, 720);
    pictureLabel->show();

    connect(pictureLabel.get(), SIGNAL(mousePressed(const QPointF&, const Qt::MouseButton&)), controller.get(), SLOT(mousePressed(const QPointF&, const Qt::MouseButton&)));
    connect(pictureLabel.get(), SIGNAL(mouseReleased(const QPointF&, const Qt::MouseButton&)), controller.get(), SLOT(mouseReleased(const QPointF&, const Qt::MouseButton&)));
    connect(pictureLabel.get(), SIGNAL(mouseMoved(const QPointF&, const Qt::MouseButton&)), controller.get(), SLOT(mouseMoved(const QPointF&, const Qt::MouseButton&)));
    connect(pictureLabel.get(), SIGNAL(keyPressed(const quint32)), controller.get(), SLOT(keyPressed(const quint32)));
    connect(pictureLabel.get(), SIGNAL(keyReleased(const quint32)), controller.get(), SLOT(keyReleased(const quint32)));

    disconnectPushButton = std::make_shared<QPushButton>("Отключиться", this);
    disconnectPushButton->setGeometry(QRect(QPoint(1300, 100), QSize(200, 50)));
    disconnectPushButton->show();

    connect(disconnectPushButton.get(), SIGNAL(clicked()), this, SLOT(disconnectPushButtonClicked()));

    recordingPushButton = std::make_shared<QPushButton>("Запись", this);
    recordingPushButton->setGeometry(QRect(QPoint(1300, 200), QSize(200, 50)));
    recordingPushButton->show();

    connect(recordingPushButton.get(), SIGNAL(clicked()), this, SLOT(recordingPushButtonClicked()));

    connect(controller.get(), SIGNAL(needUpdate()), this, SLOT(setPixmap()));
}

void MainWindow::disconnectPushButtonClicked()
{
    pictureLabel->hide();
    disconnectPushButton->hide();
    recordingPushButton->hide();
    ui->stackedWidget->setCurrentIndex(0);
    MainWindow::showNormal();

    QMetaObject::invokeMethod(controller.get(), "finish");    
}

void MainWindow::setPixmap()
{
    if (!controller->picture.isNull())
    {
        pictureLabel->setPixmap(controller->picture.scaled(pictureLabel->size()));
    }
}

void MainWindow::waitPushButtonClicked()
{
    controlled = std::make_shared<Controlled>();
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::stopSharingPushButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    QMetaObject::invokeMethod(controlled.get(), "finish");
    controlled.reset();
}

void MainWindow::recordingPushButtonClicked()
{
    if (isRecording)
    {
        recordingPushButton->setText("Запись");
        isRecording = false;
        QMetaObject::invokeMethod(controller.get(), "stopRecording");
    }
    else
    {
        recordingPushButton->setText("Остановить запись");
        isRecording = true;
        QMetaObject::invokeMethod(controller.get(), "startRecording");
    }
}
