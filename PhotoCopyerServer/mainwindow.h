#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class BoardcastServer;
class IncomingServer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
private:
    BoardcastServer *bcServer;
    IncomingServer *inServer;
};

#endif // MAINWINDOW_H
