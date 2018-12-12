#pragma once

#include "shared/qt/communication/message.h"
#include "shared/qt/communication/func_invoker.h"

#include <QtCore>
#include <QCoreApplication>
#include <QTcpSocket>
#include <QTcpServer>


class Application : public QCoreApplication
{
public:
    Application(int &argc, char **argv);

    bool init();

    static void stop() {_stop = true;}
    static bool isStopped() {return _stop;}

signals:
    void detectFacesParamsUpdated();

public slots:
    void stop2(int exitCode);
    void newConnection();
    void readyRead();
    void disconnected();
    void socketError(QAbstractSocket::SocketError);

private:
    Q_OBJECT
    void timerEvent(QTimerEvent* event) override;

private:
    int _exitCode = {0};
    int _stopTimerId = {-1};
    static volatile bool _stop;

    QTcpServer* _tcpServer;
    quint32 _blockSize = {0};

};

