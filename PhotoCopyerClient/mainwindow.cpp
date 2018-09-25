#include "connection.h"
#include "utility.h"
#include <QFileDialog>
#include <QDateTime>
#include <QGraphicsPixmapItem>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionSelectSrcFolder,SIGNAL(triggered()),this,SLOT(SelectSrcDir()));
    connect(ui->actionSelectDstFloder,SIGNAL(triggered()),this,SLOT(SelectDstDir()));
    connect(ui->actionStop,SIGNAL(triggered()),this,SLOT(StopCopy()));
    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(SetImageToScene(QModelIndex)));
    connect(ui->actionConnectTo,SIGNAL(triggered()),this,SLOT(ConnectToServer()));

    progressbar = new QProgressBar;
    this->statusBar()->addWidget(progressbar);
    md5_str_label = new QLabel;
    md5_str_label->setText("MD5 String");
    this->statusBar()->addWidget(md5_str_label);

    ui->graphicsView->setScene(&scene);
    conn = nullptr;
}

MainWindow::~MainWindow()
{
    if(conn!=nullptr)
        delete conn;
    delete ui;
}


void MainWindow::SelectSrcDir()
{
    src_folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home/huwenbo",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    if(src_folder.isEmpty())
        return;


    QHash<QString,QStringList> folder_tree;

    file_copy_therad.SetSrcDir(src_folder);
    folder_tree = file_copy_therad.GetFileTree();
    GenerateTreeModel(folder_tree);
    ui->treeView->setModel(&tree_model);
}

void MainWindow::SelectDstDir()
{
    dst_folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home/huwenbo",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dst_folder.isEmpty())
        return;


    file_copy_therad.SetProgressBar(progressbar);
    file_copy_therad.SetDstDir(dst_folder);
    file_copy_therad.start();
}

void MainWindow::StopCopy()
{
    file_copy_therad.stop();
}

void MainWindow::GenerateTreeModel(QHash<QString,QStringList> &tree)
{
    QHashIterator<QString, QStringList> hash_itor(tree);
    while(hash_itor.hasNext())
    {
        hash_itor.next();
        QStringList str_list = hash_itor.value();
        QStandardItem *item = new QStandardItem(hash_itor.key());
        tree_model.appendRow(item);
        for(int i=0;i<str_list.size();++i)
        {
            QStandardItem *sub_item = new QStandardItem(str_list[i]);
            item->appendRow(sub_item);
        }
        tree_model.appendRow(item);
    }
}

void MainWindow::SetImageToScene(const QModelIndex & index)
{
    if(index.row()>0)
    {
        int rows = index.row();
        scene.clear();

        for(int i=0;i<rows;++i)
        {
            QPixmap pix;

            pix.load(index.child(i,0).data().toString());
            pix = pix.scaled(100,100);
            QGraphicsPixmapItem *item =  scene.addPixmap(pix);
            item->setPos(i*100+10,10);

        }
    }
}

void MainWindow::ConnectToServer()
{
    if(conn==nullptr)
        conn = new Connection;
    conn->connectToHost("127.0.0.1",65000);
//    while(conn->state()!=QAbstractSocket::ConnectedState);

    conn->setGreetingMessage("Hello!");
    conn->sendGreetingMessage();
}
