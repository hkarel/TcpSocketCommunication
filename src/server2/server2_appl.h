#pragma once

#include "shared/qt/communication/message.h"
#include "shared/qt/communication/func_invoker.h"

#include <QtCore>
#include <QCoreApplication>
#include <QTcpSocket>
#include <QTcpServer>

using namespace std;
using namespace communication;

class Application : public QCoreApplication
{
public:
    Application(int &argc, char **argv);

    bool init();

    static void stop() {_stop = true;}
    static bool isStopped() {return _stop;}

public slots:
    void stop2(int exitCode);

    void message(const communication::Message::Ptr&);
    void socketConnected(communication::SocketDescriptor);
    void socketDisconnected(communication::SocketDescriptor);


    //--- Обработчики команд ---
    void command_TestCommand1(const Message::Ptr&);
    void command_TestCommand2(const Message::Ptr&);
    void command_TestCommand3(const Message::Ptr&);

private:
    Q_OBJECT
    void timerEvent(QTimerEvent* event) override;

private:
    int _exitCode = {0};
    int _stopTimerId = {-1};
    static volatile bool _stop;

    communication::FunctionInvoker _funcInvoker;
};

