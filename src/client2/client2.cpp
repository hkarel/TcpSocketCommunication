#include "client2_appl.h"
#include "shared/defmac.h"
#include "shared/utils.h"
#include "shared/logger/logger.h"
#include "shared/logger/config.h"
#include "shared/qt/logger/logger_operators.h"
#include "shared/qt/quuidex.h"
#include "shared/qt/communication/commands_base.h"
#include "shared/qt/communication/commands_pool.h"
#include "shared/qt/communication/message.h"
#include "shared/qt/communication/functions.h"
#include "shared/qt/communication/transport/tcp.h"
#include "shared/qt/communication/transport/local.h"
#include "shared/qt/compression/qlzma.h"
#include "shared/qt/compression/qppmd.h"
#include "shared/qt/config/config.h"
#include "shared/qt/version/version_number.h"
#include "shared/thread/thread_pool.h"
#include <signal.h>
#include <unistd.h>

using namespace std;
using namespace communication::transport;

/**
  Используется для уведомления основного потока о завершении работы программы.
*/
void stopProgramHandler(int sig)
{
    if ((sig == SIGTERM) || (sig == SIGINT))
    {
        const char* sigName = (sig == SIGTERM) ? "SIGTERM" : "SIGINT";
        log_verbose << "Signal " << sigName << " is received. Program will be stopped";
        Application::stop();
    }
    else
        log_verbose << "Signal " << sig << " is received";
}

void stopProgram()
{
    #define STOP_THREAD(THREAD_FUNC, NAME, TIMEOUT) \
        if (!THREAD_FUNC.stop(TIMEOUT * 1000)) { \
            log_info << "Thread '" NAME "': Timeout expired, thread will be terminated."; \
            THREAD_FUNC.terminate(); \
        }

//    STOP_THREAD(videoCapture(),         "VideoCapture",       15)
//    STOP_THREAD(detectFaces1(),         "DetectFaces1",       15)
//    STOP_THREAD(detectFaces2(),         "DetectFaces2",       15)
////    STOP_THREAD(detectFaces3(),         "DetectFaces3",       15)
//    STOP_THREAD(detectFaces4(),         "DetectFaces4",       15)
//    STOP_THREAD(statisticSender(),      "StatisticSender",    15)
//    STOP_THREAD(videoSaver(),           "VideoSaver",         15)
//    STOP_THREAD(videoSender(),          "VideoSender",        15)
//    STOP_THREAD(videoCollector(),       "VideoCollector",     15)

//    mvnc::mvdetector().deinit();

    //tcp::listener().close();
    //local::listener().close();

    log_info << "'Client2' service is stopped";
    alog::logger().stop();

    //trd::threadPool().stop();

    #undef STOP_THREAD
}

//void helpInfo(/*const char * binary*/)
//{
//    alog::logger().clearSavers();
//    alog::logger().addSaverStdOut(alog::Level::Info, true);

//    log_info << "'Tochka Faces' service"
//             << " (version: " << productVersion().toString()
//             << "; binary protocol version: "
//             << BPROTOCOL_VERSION_LOW << "-" << BPROTOCOL_VERSION_HIGH
//             << "; gitrev: " << GIT_REVISION << ")";
//    log_info << "Usage: tochkaf [nh]";
//    //log_info << "Usage: 27fretaild [-c <config file>] [-l <log file>] [-n] [-h]";
//    //log_info << "  -c config file (default /etc/27FacesRetail/27fretaild.conf)";
//    //log_info << "  -l log file (default /var/opt/27FacesRetail/log/27fretaild.log)";
//    log_info << "  -n do not daemonize";
//    log_info << "  -h this help";
//    alog::logger().flush();
//}

int main(int argc, char *argv[])
{
//    QString str = "Hello, hello world!";
//    QFile file {"/tmp/server1.dump"};
//    if (file.open(QIODevice::WriteOnly))
//    {
//        QDataStream stream {&file};
//        //stream.setByteOrder(QDataStream::LittleEndian);
//        stream.setVersion(QDataStream::Qt_4_8);
//        stream << str.toUtf8();
//        file.close();
//    }


    // Устанавливаем в качестве разделителя целой и дробной части символ '.',
    // если этого не сделать - функции преобразования строк в числа (std::atof)
    // буду неправильно работать.
    qputenv("LC_NUMERIC", "C");

    // Пул потоков запустим после кода демонизации
    //trd::threadPool().stop();

    int ret = 0;
    try
    {
        //google::InitGoogleLogging(argv[0]);
        alog::logger().start();

#ifdef NDEBUG
        alog::logger().addSaverStdOut(alog::Level::Info, true);
#else
        alog::logger().addSaverStdOut(alog::Level::Debug);
#endif
        signal(SIGTERM, &stopProgramHandler);
        signal(SIGINT,  &stopProgramHandler);

        QDir homeDir = QDir::home();
        if (!homeDir.exists())
        {
            log_error << "Home dir " << homeDir.path() << " not exists";
            alog::logger().stop();
            return 1;
        }

        //QString configFile;
        //QString logFileArg;
        bool isDaemon = true;

        int c;
        while ((c = getopt(argc, argv, "nhc:l:")) != EOF)
        {
            switch(c)
            {
                //case 'h':
                //    helpInfo();
                //    alog::logger().stop();
                //    exit(0);
                //case 'c':
                //    configFile = optarg;
                //    break;
                //case 'l':
                //    logFileArg = optarg;
                //    break;
                case 'n':
                    isDaemon = false;
                    break;
              case '?':
                    log_error << "Invalid option";
                    alog::logger().stop();
                    return 1;
            }
        }

//        google::InitGoogleLogging(argv[0]);

//        QString configFile = "/etc/tochka/tochkaf.conf";
//        if (!QFile::exists(configFile))
//        {
//            log_error << "Config file " << configFile << " not exists";
//            alog::logger().stop();
//            return 1;
//        }

//        config::base().setReadOnly(true);
//        config::base().setSaveDisabled(true);
//        if (!config::base().read(configFile.toStdString()))
//        {
//            alog::logger().stop();
//            return 1;
//        }

//        QString configFileS = "/var/opt/tochka/state/tochkaf.state";
//        config::state().read(configFileS.toStdString());

        QString logFile;
        logFile = "/tmp/client2.log";

//        config::base().getValue("logger.file", logFile);

//        config::homeDirExpansion(logFile);

//        QFileInfo logFileInfo {logFile};
//        QString logFileDir = logFileInfo.absolutePath();
//        if (!QDir(logFileDir).exists())
//            if (!QDir().mkpath(logFileDir))
//            {
//                log_error << "Failed create log directory: " << logFileDir;
//                alog::logger().stop();
//                return 1;
//            }

        // Создаем дефолтный сэйвер для логгера
        {
            std::string logLevelStr = "debug2";
            //config::base().getValue("logger.level", logLevelStr);

            bool logContinue = true;
            //config::base().getValue("logger.continue", logContinue);

            alog::Level logLevel = alog::levelFromString(logLevelStr);
            alog::SaverPtr saver {new alog::SaverFile("default",
                                                      logFile.toStdString(),
                                                      logLevel,
                                                      logContinue)};
            alog::logger().addSaver(saver);
        }
        log_info << "'Client2' service is running";
        alog::logger().flush();

        if (isDaemon)
        {
            alog::logger().stop();
            alog::logger().removeSaverStdOut();
            alog::logger().removeSaverStdErr();

            if (daemon(1, 0) != 0)
                return 0;

            alog::logger().start();
            log_verbose << "Demonization success";
        }
        alog::logger().removeSaverStdOut();
        alog::logger().removeSaverStdErr();

//        // Создаем дополнительные сэйверы для логгера
//        QString logConf;
//        config::base().getValue("logger.conf", logConf);

//        if (!logConf.isEmpty())
//        {
//            config::homeDirExpansion(logConf);
//            if (QFile::exists(logConf))
//                alog::loadSavers(logConf.toStdString());
//            else
//                log_error << "Logger config file not exists: " << logConf;
//        }
//        alog::printSaversInfo();

        if (!communication::commandsPool().checkUnique())
        {
            stopProgram();
            return 1;
        }

        // Пул потоков нужно активировать после кода демонизации
        //trd::threadPool().start();

        Application appl {argc, argv};

        // Устанавливаем текущую директорию. Эта конструкция работает только
        // когда создан экземпляр QCoreApplication.
        if (QDir::setCurrent(QCoreApplication::applicationDirPath()))
        {
            log_debug << "Set work directory: " << QCoreApplication::applicationDirPath();
        }
        else
        {
            log_error << "Failed set work directory";
            stopProgram();
            return 1;
        }

//        // Инициализация communication::listener::tcp
//        QHostAddress hostAddress = QHostAddress::Any;
//        QString hostAddressStr;
//        if (config::base().getValue("listener.address", hostAddressStr))
//        {
//            if (hostAddressStr.toLower().trimmed() == "localhost")
//                hostAddress = QHostAddress::LocalHost;
//            else if (hostAddressStr.toLower().trimmed() == "any")
//                hostAddress = QHostAddress::Any;
//            else
//                hostAddress = QHostAddress(hostAddressStr);
//        }
//        int port = 61056;
//        config::base().getValue("listener.port", port);
//        if (!tcp::listener().init({hostAddress, port}))
//        {
//            stopProgram();
//            return 1;
//        }
//        chk_connect_q(&tcp::listener(), SIGNAL(message(communication::Message::Ptr)),
//                      &appl, SLOT(message(communication::Message::Ptr)))

//        chk_connect_q(&tcp::listener(), SIGNAL(socketConnected(communication::SocketDescriptor)),
//                      &appl, SLOT(socketConnected(communication::SocketDescriptor)))

//        chk_connect_q(&tcp::listener(), SIGNAL(socketDisconnected(communication::SocketDescriptor)),
//                      &appl, SLOT(socketDisconnected(communication::SocketDescriptor)))

        QMetaObject::invokeMethod(&appl, "sendCommand", Qt::QueuedConnection);
        ret = appl.exec();
    }
    catch (std::exception& e)
    {
        log_error << "Failed initialization. Detail: " << e.what();
        ret = 1;
    }
    catch (...)
    {
        log_error << "Failed initialization. Unknown error";
        ret = 1;
    }

    stopProgram();
    return ret;
}
