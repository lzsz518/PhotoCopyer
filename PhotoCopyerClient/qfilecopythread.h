#ifndef QFILECOPYTHREAD_H
#define QFILECOPYTHREAD_H

#include <QThread>
#include <QString>
#include <QHash>
#include <QStringList>

class QProgressBar;

class QFileCopyThread : public QThread
{
public:
    QFileCopyThread();
    ~QFileCopyThread();

    void SetSrcDir(const QString &folder);
    void SetDstDir(const QString &folder);
    void SetProgressBar(QProgressBar *pb);
    QString GetSrcDir() const;
    QString GetDstDir() const;

    QStringList GetFileList() const;

    void stop();

    QHash<QString,QStringList> GetFileTree();
private:
    bool run_flag;
    QString src_folder;
    QString dst_folder;

    QStringList src_folder_files;
    QHash<QString,QStringList> folder_tree;
    QProgressBar *progress_bar;

    void run();
};

#endif // QFILECOPYTHREAD_H
