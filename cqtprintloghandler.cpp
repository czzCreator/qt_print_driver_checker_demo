#include "cqtprintloghandler.h"

#include <QThread>
#include <QMessageBox>

#include <unistd.h>

#define FLAG_EFFICIENCY "efficiency_" //to discriminate the efficiency log
static CQtPrintLogHandler* g_Log = nullptr;
static QString g_qsLogPath = "";
static QFile g_logfile;
static QString g_qsEffLogPath = "";
static bool g_bEfficiency = false;
#define MAX_LOGS 10

CQtPrintLogHandler::CQtPrintLogHandler()
{
}

CQtPrintLogHandler::~CQtPrintLogHandler()
{
}

CQtPrintLogHandler* CQtPrintLogHandler::GetLog()
{
    if(g_Log == nullptr)
    {
        g_Log = new CQtPrintLogHandler;
    }
    return g_Log;
}

void CQtPrintLogHandler::FunctionEnd(QString funName)
{
    m_EndTime = QTime::currentTime().msecsSinceStartOfDay();
    int msec = m_EndTime - m_StartTime;
//    funName.append(QString("UsedTime: %1ms  ").arg(msec));

    QString qsOut = QString("[TimeStamp]-----%1-----end-----UsedTime: %2ms ").arg(funName).arg(msec);

    LOG_DEBUG(qPrintable(qsOut));
}

void CQtPrintLogHandler::FunctionStart(QString funName)
{
    m_StartTime = QTime::currentTime().msecsSinceStartOfDay();
    QString qsOut = QString("[TimeStamp]-----%1-----begin").arg(funName);
    LOG_DEBUG(qPrintable(qsOut));
}

void CQtPrintLogHandler::FunctionStart_Efficiency(QString funName)
{
#ifndef QT_DEBUG
    if(!g_bEfficiency)return;
#endif

    m_StartTime = QTime::currentTime().msecsSinceStartOfDay();
    storeTime.insert(funName, m_StartTime);
    QString qsOut = QString("[TimeStamp]-----%1-----begin").arg(funName);
    QString flagAndOut = QString(FLAG_EFFICIENCY).append(qsOut);

    LOG_DEBUG(qPrintable(flagAndOut));
}

void CQtPrintLogHandler::FunctionEnd_Efficiency(QString funName)
{
#ifndef QT_DEBUG
    if(!g_bEfficiency)return;
#endif

    m_EndTime = QTime::currentTime().msecsSinceStartOfDay();
    int msec;
    if(storeTime.contains(funName))
    {
         msec = m_EndTime - storeTime[funName];
    }
    else
    {
        LOG_DEBUG(qPrintable(QString("There is no match start time for funName %1").arg(funName)));
        return;
    }

    QString qsOut = QString("[TimeStamp]-----%1-----end-----UsedTime: %2ms ").arg(funName).arg(msec);
    QString flagAndOut = QString(FLAG_EFFICIENCY).append(qsOut);

    LOG_DEBUG(qPrintable(flagAndOut));
}

void CQtPrintLogHandler::LogMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug");
        break;
    case QtInfoMsg:
        text = QString("Info");
        break;

    case QtWarningMsg:
        text = QString("Warning");
        break;

    case QtCriticalMsg:
        text = QString("Critical");
        break;

    case QtFatalMsg:
        text = QString("Fatal");
    default:
        break;
    }

    int dwPID = getpid();
    Qt::HANDLE dwThreadID = QThread::currentThreadId();

    text = QString("%1 [%2] [%3-%4] ").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(text)
            .arg(dwPID)
            .arg((quint64)dwThreadID);

    int index = msg.lastIndexOf('\\');
    QString msgNew = msg.right(msg.length()-index-1);

    //clear the log when first open the logfile
    static bool bfirstOpen = true;
    static bool bFirstOpenEfficency = true;

    QIODevice::OpenMode openMode = QIODevice::ReadWrite | QIODevice::Text;
    bool bEfficiency = (msgNew.indexOf(FLAG_EFFICIENCY) == 0)?true:false;
    if(bEfficiency && g_bEfficiency)
    {
        msgNew = msgNew.remove(FLAG_EFFICIENCY);
        g_logfile.setFileName(g_qsEffLogPath);
        if(bFirstOpenEfficency)
            openMode |= QIODevice::Truncate;
        else
            openMode |= QIODevice::Append;
    }
    else
    {
        g_logfile.setFileName(g_qsLogPath);
        if(bfirstOpen)
            openMode |= QIODevice::Truncate;
        else
            openMode |= QIODevice::Append;
    }

    bool bOpenSuccess = g_logfile.open(openMode);
    if(bOpenSuccess)
    {
        if(bEfficiency)
            bFirstOpenEfficency = false;
        else
            bfirstOpen = false;

        QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString current_date = QString("(%1)").arg(current_date_time);
//        QString message = QString("%1 %2 %3").arg(text).arg(msgNew).arg(current_date);
        QString okmessage = QString("%1 %2").arg(text).arg(msgNew);

        QTextStream txtOutput(&g_logfile);
        txtOutput << okmessage << "\r\n";

        CQtPrintLogHandler::GetLog()->sig_message_log(okmessage);

        g_logfile.flush();
        g_logfile.close();
    }
    else {
        QMessageBox::warning(nullptr,QString("Error Message"),QString("Oops ,open log file error:%1").arg(g_logfile.errorString()));
    }

    mutex.unlock();
}

void CQtPrintLogHandler::setLogFilePath(const QString &logFilePath)
{
    g_qsLogPath = logFilePath;
}

QString CQtPrintLogHandler::getLogFilePath()
{
    return g_qsLogPath;
}










