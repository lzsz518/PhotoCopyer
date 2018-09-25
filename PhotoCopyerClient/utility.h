#ifndef UTILITY
#define UTILITY
#include <QString>
#include <QFileInfo>
#include <string>
#include <QHash>

using namespace std;

#define FILE_SAME 0
#define FILE_NOT_SAME 1
#define FILE_FORMAT_INCORRECT 2
#define FILE_DOES_NOT_EXIST 3

#ifdef __cplusplus
extern "C"
{
#endif

int GetFolderImages(const QString path, QStringList &string_list,QHash<QString,QStringList> &folder_tree);

QString GenerateFolderName(const QFileInfo &fi);

int ImageCompare(const QString &file1,const QString &file2);

int MoveCompare(const QString &file1, const QString &file2);

long long GetFileLength(const QString &file);

bool IsImageFile(const QFileInfo &fi);

bool IsMoveFile(const QFileInfo &fi);

unsigned char* GenerateImageMD5(const QString &file);

#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif // UTILITY

