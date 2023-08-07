#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

#include "cqtprintloghandler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setMenuBar(nullptr);
    setStatusBar(nullptr);

    m_printer = new QPrinter;
    m_printDialog = new QPrintDialog(m_printer,this);
    m_printDialog->setWindowTitle(tr("Print Document Test"));

    ui->lineEdit->setText(CQtPrintLogHandler::GetLog()->getLogFilePath());

    connect(ui->simulate_init_btn,&QPushButton::clicked,this,[=](){

        ui->tabWidget->setCurrentWidget(ui->Log_tab);

        reInit();

        if(m_printDialog->exec() != QDialog::Accepted)
            return;

        qDebug() <<"/**********************************************************************************************/";
        qDebug() <<"    ready to simulate  QPainter  init  with  current  select  printer   ";
        qDebug() <<"    will out put an empty pdf file to test...........    ";
        qDebug() <<"    current printerName:   " << m_printer->printerName();
        QString current_out_fileName = m_printer->outputFileName();

        if(!current_out_fileName.isEmpty())
            qDebug() <<"    current m_printer->outputFileName():   " << m_printer->outputFileName();
        else
        {
            QString printOut_textName = ui->lineEdit_2->text();
            QFileInfo txtToPrint(printOut_textName);

            if(printOut_textName.isEmpty() || !txtToPrint.exists())
            {
                QMessageBox::warning(nullptr,tr("warning"),tr("Oops,printOut_textName Empty,select a 'txt' format file toBe printed first......or file not exist......"));
                return;
            }

            QString baseName = txtToPrint.completeBaseName();
            QString out_pdf_file_name = txtToPrint.absoluteDir().absolutePath() + "/" + baseName + ".pdf";

            if(ui->checkBox->isChecked())
            {
                m_printer->setOutputFormat(QPrinter::PdfFormat);
                m_printer->setOutputFileName(out_pdf_file_name);
            }

            qDebug() <<"    find current outputFileName empty......,now set to:----->   " << out_pdf_file_name;
        }

        qDebug() <<"/**********************************************************************************************/";

        m_simulate_init_printPainter = new QPainter;
        if(!m_simulate_init_printPainter->begin(m_printer))
        {
            qDebug() <<"Oops,printPainter begin failed upper qt framework ------------ currentPrinter->isValid(): " << m_printer->isValid();
            qDebug() <<"printPainter->begin fail";
            return;
        }

        m_simulate_init_printPainter->end();
    });

    connect(ui->pushButton,&QPushButton::clicked,this,[=](){

        reInit();

        if(m_printDialog->exec() != QDialog::Accepted)
            return;

        QString printOut_textName = ui->lineEdit_2->text();
        QFileInfo txtToPrint(printOut_textName);

        if(printOut_textName.isEmpty() || !txtToPrint.exists())
        {
            QMessageBox::warning(nullptr,tr("warning"),tr("Oops,printOut_textName Empty,select a 'txt' format file toBe printed first......or file not exist......"));
            return;
        }

        QString baseName = txtToPrint.completeBaseName();
        QString out_pdf_file_name = txtToPrint.absoluteDir().absolutePath() + "/" + baseName + ".pdf";

        qDebug() << "ready to print out file Path------>:" << out_pdf_file_name;

        qDebug() << "Print Configuration: PrinterName--->" << m_printer->printerName();
        qDebug() << "Print Configuration: printRange--->" << m_printer->printRange() ;
        qDebug() << "Print Configuration: printerState--->" << m_printer->printerState();

        // l/r/t/b 分别代表上下左右页边距
        qDebug() << "Print Configuration: pageSize(纸张大小) Index: (0==A4)--->" << m_printer->pageSize();
        qDebug() << "Print Configuration: PageLayout--->" << m_printer->pageLayout();
        qDebug() << "Print Configuration: copyCount--->" << m_printer->copyCount();
        qDebug() << "Print Configuration: outputFormat--->" << m_printer->outputFormat();

        ui->tabWidget->setCurrentWidget(ui->Log_tab);

        if(ui->checkBox->isChecked())
        {
            m_printer->setOutputFormat(QPrinter::PdfFormat);
            m_printer->setOutputFileName(out_pdf_file_name);
        }

        ui->preview_txt_show->print(m_printer);
    });

    connect(ui->pushButton_2,&QPushButton::clicked,this,[=](){
        QString openfileName = QFileDialog::getSaveFileName(this,QString("select path for log file to Store"),QApplication::applicationDirPath()+"/printLog.txt","Text (*.txt)");

        if(openfileName.isEmpty())
            return;

        ui->lineEdit->setText(openfileName);
        CQtPrintLogHandler::GetLog()->setLogFilePath(openfileName);

        ui->tabWidget->setCurrentWidget(ui->Log_tab);
    });

    connect(ui->btn_browser_file,&QPushButton::clicked,this,[=](){

        QString openfileName = QFileDialog::getOpenFileName(this,QString("select a txt file to print test"),QApplication::applicationDirPath(),"Text (*.txt)");

        if(openfileName.isEmpty())
            return;

        QFile txtFile(openfileName);
        if(!txtFile.exists())
            return;

        ui->lineEdit_2->setText(openfileName);

        if(!txtFile.open(QIODevice::ReadOnly))
        {
            qWarning() << QString("Oops,open file: %1 failed,---------reason: %2").arg(openfileName).arg(txtFile.errorString());
            return;
        }

        QByteArray all_text_string = txtFile.readAll();
        ui->preview_txt_show->setText(QString(all_text_string));

        ui->tabWidget->setCurrentWidget(ui->View_tab);
        txtFile.close();
    });

    connect(ui->clear_btn,&QPushButton::clicked,this,[=](){

        ui->tabWidget->setCurrentWidget(ui->Log_tab);
        ui->logShow_textEdit->clear();
    });

    CQtPrintLogHandler *pGlobalLogger = CQtPrintLogHandler::GetLog();
    connect(pGlobalLogger,&CQtPrintLogHandler::sig_message_log,this,[=](const QString mess){
        ui->logShow_textEdit->append(mess);
    });

    resize(1150,600);
}

MainWindow::~MainWindow()
{
    delete ui;

    if(m_printer)
    {
        delete m_printer;
        m_printer = Q_NULLPTR;
    }

    if(m_simulate_init_printPainter)
    {
        delete m_simulate_init_printPainter;
        m_simulate_init_printPainter = Q_NULLPTR;
    }
}

void MainWindow::reInit()
{
    if(m_simulate_init_printPainter){
        delete m_simulate_init_printPainter;
        m_simulate_init_printPainter=Q_NULLPTR;
    }

    if(m_printer)
    {
        delete m_printer;
        m_printer = Q_NULLPTR;
    }

    if(m_printDialog)
    {
        delete m_printDialog;
        m_printDialog = Q_NULLPTR;

        m_printer = new QPrinter;
        m_printDialog = new QPrintDialog(m_printer,this);
        m_printDialog->setWindowTitle(tr("Print Document Test"));
    }
}





