#include "server2_appl.h"
#include "shared/logger/logger.h"
#include "shared/qt/logger/logger_operators.h"
#include "shared/qt/communication/functions.h"
#include "shared/qt/communication/transport/base.h"
#include "shared/qt/communication/transport/local.h"
#include "shared/qt/communication/transport/tcp.h"
#include "shared/qt/communication/commands_pool.h"
#include "shared/qt/communication/logger_operators.h"
#include "../communication/commands.h"

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


    FUNC_REGISTRATION(TestCommand1)
    FUNC_REGISTRATION(TestCommand2)
    FUNC_REGISTRATION(TestCommand3)
    _funcInvoker.sort();

    #undef FUNC_REGISTRATION
}

void Application::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == _stopTimerId)
    {
        if (_stop)
            exit(_exitCode);
    }
}

void Application::stop2(int exitCode)
{
    _exitCode = exitCode;
    stop();
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

void Application::command_TestCommand1(const Message::Ptr& message)
{
    log_info_m << "Message TestCommand1 was received";
}

void Application::command_TestCommand2(const Message::Ptr& message)
{
    log_info_m << "Message TestCommand2 was received";

    data::TestCommand2 testCommand2;
    readFromMessage(message, testCommand2);
    log_info_m << "testCommand2.a " << testCommand2.a;
    log_info_m << "testCommand2.b " << testCommand2.b;
}

void Application::command_TestCommand3(const Message::Ptr& message)
{
    log_info_m << "Message TestCommand3 was received";

    data::TestCommand3 testCommand3;
    readFromMessage(message, testCommand3);
    log_info_m << "testCommand3.m " << testCommand3.m;
    log_info_m << "testCommand3.k " << testCommand3.k;

    for (const data::Point& p : testCommand3.points)
    {
        log_info_m << "testCommand3 point item:"
                   << " x: " << p.x
                   << " y: " << p.y;
    }
}


#undef log_error_m
#undef log_warn_m
#undef log_info_m
#undef log_verbose_m
#undef log_debug_m
#undef log_debug2_m
