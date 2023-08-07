#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewWidget>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void reInit();

private:
    Ui::MainWindow *ui;

    QPrinter *m_printer = Q_NULLPTR;
    QPrintDialog *m_printDialog = Q_NULLPTR;

    QPainter *m_simulate_init_printPainter = Q_NULLPTR;      //painter
};

#endif // MAINWINDOW_H
