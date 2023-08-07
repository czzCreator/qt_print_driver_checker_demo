#ifndef CQTPRINTLOGHANDLER_H
#define CQTPRINTLOGHANDLER_H

#include <QDebug>
#include <QFile>
#include <QTime>

/************************************************************************/
/*                                                                      */
/************************************************************************/

#define LOG_INFO    QString("File: %1, Line: %2, Function: %3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__)
//#define LOG_DEBUG(format,...) qDebug(format, ##__VA_ARGS__) //format string too long may crash
#define LOG_DEBUG(format) qDebug()<<format

#define LOG_OUTPUT(format,...) qDebug("File: " __FILE__ ", Line: %05d, Function: %s, " format, __LINE__, __FUNCTION__, ##__VA_ARGS__)
//#define LOG_OUTPUT(format,...) qDebug(" Line: %05d, Function: %s, "format, __LINE__, __FUNCTION__, ##__VA_ARGS__)



class CQtPrintLogHandler: public QObject
{
    Q_OBJECT
public:
    CQtPrintLogHandler();
    ~CQtPrintLogHandler();

    void FunctionStart(QString funName);
    void FunctionEnd(QString funName);
    void FunctionStart_Efficiency(QString funName);
    void FunctionEnd_Efficiency(QString funName);

    static CQtPrintLogHandler* GetLog();
    static void LogMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void setLogFilePath(const QString &logFilePath);
    static QString getLogFilePath();

signals:
    void sig_message_log(const QString &messInfo);

private:
    int m_StartTime;
    int m_EndTime;
    QMap<QString, int> storeTime;
};

#endif // CQTPRINTLOGHANDLER_H
