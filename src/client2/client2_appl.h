#pragma once

#include "shared/qt/communication/message.h"
#include "shared/qt/communication/func_invoker.h"
#include "shared/qt/communication/transport/tcp.h"

#include <QtCore>
#include <QCoreApplication>

using namespace std;
using namespace communication;
using namespace communication::transport;

class Application : public QCoreApplication
{
public:
    Application(int &argc, char **argv);

    bool init();

    static void stop() {_stop = true;}
    static bool isStopped() {return _stop;}

public slots:
    void stop2(int exitCode);
    void sendCommand();

    void message(const communication::Message::Ptr&);
    void socketConnected(communication::SocketDescriptor);
    void socketDisconnected(communication::SocketDescriptor);

private:
    Q_OBJECT
    void timerEvent(QTimerEvent* event) override;

private:
    int _exitCode = {0};
    int _stopTimerId = {-1};
    static volatile bool _stop;

    tcp::Socket::Ptr _socket;
    communication::FunctionInvoker _funcInvoker;
};

