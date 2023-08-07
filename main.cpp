#include "mainwindow.h"
#include <QApplication>
#include <QStandardPaths>

#include "cqtprintloghandler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList paths_all = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    if(paths_all.size() == 0)
        CQtPrintLogHandler::GetLog()->setLogFilePath(QApplication::applicationDirPath() + "/printLog.txt");
    else
        CQtPrintLogHandler::GetLog()->setLogFilePath(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).at(0) + "/printLog.txt");

    qInstallMessageHandler(CQtPrintLogHandler::LogMessage);

    MainWindow w;
    w.show();

    return a.exec();
}
