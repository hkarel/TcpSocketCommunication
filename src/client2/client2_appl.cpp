#include "client2_appl.h"
#include "shared/logger/logger.h"
#include "shared/qt/logger/logger_operators.h"
#include "shared/qt/communication/functions.h"
#include "shared/qt/communication/transport/base.h"
#include "shared/qt/communication/transport/local.h"
#include "shared/qt/communication/transport/tcp.h"
#include "shared/qt/communication/commands_pool.h"
#include "shared/qt/communication/logger_operators.h"
#include "../communication/commands.h"

#include <unistd.h>

#define log_error_m   alog::logger().error_f  (__FILE__, LOGGER_FUNC_NAME, __LINE__, "Application")
#define log_warn_m    alog::logger().warn_f   (__FILE__, LOGGER_FUNC_NAME, __LINE__, "Application")
#define log_info_m    alog::logger().info_f   (__FILE__, LOGGER_FUNC_NAME, __LINE__, "Application")
#define log_verbose_m alog::logger().verbose_f(__FILE__, LOGGER_FUNC_NAME, __LINE__, "Application")
#define log_debug_m   alog::logger().debug_f  (__FILE__, LOGGER_FUNC_NAME, __LINE__, "Application")
#define log_debug2_m  alog::logger().debug2_f (__FILE__, LOGGER_FUNC_NAME, __LINE__, "Application")

using namespace communication;
using namespace communication::transport;

volatile bool Application::_stop = false;

Application::Application(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
    _stopTimerId = startTimer(1000);

    #define FUNC_REGISTRATION(COMMAND) \
        _funcInvoker.registration(command:: COMMAND, &Application::command_##COMMAND, this);

////    FUNC_REGISTRATION(GetWebcamParams)
////    FUNC_REGISTRATION(WebcamParams)
    _funcInvoker.sort();

    #undef FUNC_REGISTRATION

    _socket = tcp::Socket::Ptr{new tcp::Socket};
}

void Application::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == _stopTimerId)
    {
        if (_stop)
        {
            _socket->stop();
            exit(_exitCode);
        }
    }
}

void Application::stop2(int exitCode)
{
    _exitCode = exitCode;
    stop();
}

void Application::sendCommand()
{
    QHostAddress hostAddress = QHostAddress::LocalHost;
    int port = 62050;
    if (!_socket->init({hostAddress, port}))
    {
        log_error_m << "Failed initialize a communication system";
        return;
    }
    //_socket->setCheckProtocolCompatibility(false);
    _socket->setMessageFormat(SerializationFormat::Json);
    _socket->connect();

    int sleepCount = 0;
    while (sleepCount++ < 20)
    {
        usleep(50*1000);
        qApp->processEvents();
        if (_socket->isConnected())
            break;
    }

    if (!_socket->isConnected())
    {
        _socket->stop();
        log_error_m << "Failed connect to host: " << _socket->peerPoint();
        return;
    }
    Message::Ptr m = createMessage(command::TestCommand1);
    _socket->send(m);
    log_info_m << "Message TestCommand1 was sended";


    data::TestCommand2 testCommand2;
    testCommand2.a = 15;
    testCommand2.b = 16;

    Message::Ptr m2 = createJsonMessage(testCommand2);
    _socket->send(m2);

    data::TestCommand3 testCommand3;
    testCommand3.m = 105;
    testCommand3.k = 200;
    testCommand3.points.append({10, 11});
    testCommand3.points.append({20, 21});
    testCommand3.points.append({30, 31});

    Message::Ptr m3 = createJsonMessage(testCommand3);
    _socket->send(m3);

}

void Application::message(const Message::Ptr& message)
{
    if (message->processed())
        return;

    if (_funcInvoker.containsCommand(message->command()))
    {
        if (!commandsPool().commandIsMultiproc(message->command()))
            message->markAsProcessed();
        _funcInvoker.call(message);
    }
}

void Application::socketConnected(SocketDescriptor socketDescriptor)
{
//    data::StatusVideoSave statusVideoSave;
//    statusVideoSave.active = videoSaver().isRunning();
//    Message::Ptr m = createMessage(statusVideoSave);
//    m->destinationSocketDescriptors().insert(socketDescriptor);
//    listenerSend(m);
}

void Application::socketDisconnected(SocketDescriptor socketDescriptor)
{
//    if (_videoSaverSocketDescriptor == socketDescriptor)
//        _videoSaverSocketDescriptor = 0;
}


#undef log_error_m
#undef log_warn_m
#undef log_info_m
#undef log_verbose_m
#undef log_debug_m
#undef log_debug2_m
