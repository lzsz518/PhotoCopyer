#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QStringListModel>
#include <QProgressBar>
#include <QLabel>
#include <QStandardItemModel>
#include "qfilecopythread.h"
#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class Connection;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString src_folder;
    QString dst_folder;

    QStringListModel src_folder_model;
    QStringList src_folder_files;
    QProgressBar *progressbar ;
    QLabel *md5_str_label;
    QFileCopyThread file_copy_therad;
    QStandardItemModel tree_model;
    QGraphicsScene scene;
    Connection *conn;
    void GenerateTreeModel(QHash<QString,QStringList> &tree);
private slots:
    void SelectSrcDir();
    void SelectDstDir();
    void StopCopy();
    void SetImageToScene(const QModelIndex & index);
    void ConnectToServer();
};

#endif // MAINWINDOW_H
