#include "server1_appl.h"
#include "shared/logger/logger.h"
#include "shared/qt/logger/logger_operators.h"
#include "shared/qt/communication/functions.h"
#include "shared/qt/communication/transport/base.h"
#include "shared/qt/communication/transport/local.h"
#include "shared/qt/communication/transport/tcp.h"
#include "shared/qt/communication/commands_pool.h"

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

    /*
    #define FUNC_REGISTRATION(COMMAND) \
        _funcInvoker.registration(command:: COMMAND, &Application::command_##COMMAND, this);
    */

////    FUNC_REGISTRATION(GetWebcamParams)
////    FUNC_REGISTRATION(WebcamParams)
////    FUNC_REGISTRATION(ShowPolylineIntersect)
//    FUNC_REGISTRATION(DetectFacesParams)
////    FUNC_REGISTRATION(TokenKey)
//    FUNC_REGISTRATION(StartVideoSave)
//    FUNC_REGISTRATION(StopVideoSave)
//    _funcInvoker.sort();

//    #undef FUNC_REGISTRATION
}

bool Application::init()
{
    _tcpServer = new QTcpServer(this);
    if (!_tcpServer->listen(QHostAddress::Any, 62095))
    {
        log_error_m << "Start listener of connection to is failed";
        return false;
    }

    chk_connect_a(_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()))
    return true;
}

void Application::newConnection()
{
    QTcpSocket* clientConnection = _tcpServer->nextPendingConnection();
    chk_connect_a(clientConnection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    chk_connect_a(clientConnection, SIGNAL(readyRead()), this, SLOT(readyRead()));
    chk_connect_a(clientConnection, SIGNAL(error(QAbstractSocket::SocketError)),
                  this, SLOT(socketError(QAbstractSocket::SocketError)))

}

void Application::disconnected()
{
    QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());
    tcpSocket->deleteLater();
    _blockSize = 0;
}

void Application::readyRead()
{
    QTcpSocket* tcpSocket = qobject_cast<QTcpSocket*>(sender());

    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_8);

    if (_blockSize == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint32))
            return;

        in >> _blockSize;
        log_info_m << "BlockSize: " << _blockSize;
    }

    if (tcpSocket->bytesAvailable() < _blockSize)
        return;

    QByteArray ba;
    ba.resize(_blockSize);

    tcpSocket->read((char*)ba.constData(), _blockSize);
    log_info_m << "Socket message: " << ba;
}

void Application::socketError(QAbstractSocket::SocketError err)
{
    switch (err)
    {
        case QAbstractSocket::RemoteHostClosedError:
            log_error_m << "RemoteHostClosedError";
            break;

        case QAbstractSocket::HostNotFoundError:
            log_error_m << "HostNotFoundError";
            break;

        case QAbstractSocket::ConnectionRefusedError:
            log_error_m << "ConnectionRefusedError";
            break;

        default:
            log_error_m << "Socket error: "; // << tcpSocket->errorString();
    }

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


#undef log_error_m
#undef log_warn_m
#undef log_info_m
#undef log_verbose_m
#undef log_debug_m
#undef log_debug2_m
