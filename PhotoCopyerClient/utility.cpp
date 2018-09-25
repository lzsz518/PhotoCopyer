#include <fstream>
#include <QDir>
#include <QDirIterator>
#include <QHash>
#include <QDateTime>
#include "md5.h"
#include "opencv2/highgui/highgui.hpp"
#include "exif.h"
#include "utility.h"


using namespace cv;


int GetFolderImages(const QString path, QStringList &string_list,QHash<QString,QStringList> &folder_tree)
{
    //判断路径是否存在
    string_list.clear();
    folder_tree.clear();
    int result = 0;
    QDir dir(path);
    if(!dir.exists())
    {
        return result;
    }

    //获取所选文件类型过滤器
    QStringList filters;
    filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp")
          <<QString("*.mov")<<QString("*.mp4");

    //定义迭代器并设置过滤器
    QDirIterator dir_iterator(path,
        filters,
        QDir::Files | QDir::NoSymLinks,
        QDirIterator::Subdirectories);

    //QHash<QString,QStringList> folder_tree;
    QString folder_name;
    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        QString absolute_file_path = file_info.absoluteFilePath();
        string_list.append(absolute_file_path);
        folder_name = GenerateFolderName(file_info);
        if(folder_tree.contains(folder_name))
        {
            folder_tree.find(folder_name).value().append(absolute_file_path);
        }
        else
        {
            QHash<QString,QStringList>::iterator itor = folder_tree.insert(folder_name,QStringList());
            itor.value().append(absolute_file_path);

        }
        ++result;
    }
    return result;
}


QString GenerateFolderName(const QFileInfo &fi)
{

    QString dd;
    QString mm;
    QString yyyy;
    QString result;

    QString last_modify_date = fi.lastModified().toString("dd.MM.yyyy");
    dd = last_modify_date.mid(0,2);
    mm = last_modify_date.mid(3,2);
    yyyy = last_modify_date.mid(6,4);
    result = yyyy + QString(".") + mm + QString((".")) + dd;

    if(fi.suffix().toUpper()==QString("MOV")||fi.suffix().toUpper()==QString("MP4"))
        return result;

    FILE *fp = fopen(fi.absoluteFilePath().toStdString().c_str(), "rb");
    if(fp!=nullptr)
    {
        fseek(fp, 0, SEEK_END);
        unsigned long fsize = ftell(fp);
        rewind(fp);
        unsigned char *buf = new unsigned char[fsize];
        if(fread(buf, 1, fsize, fp) != fsize) {
            delete []buf;
            fclose(fp);
            return  result;
        }
        fclose(fp);

        // Parse exif
        EXIFInfo exif_info;
        ParseEXIF(buf, fsize, exif_info);

        if(exif_info.dateTimeOriginal==0)
            return result;

        QString original_date = exif_info.dateTimeOriginal;

        dd = original_date.mid(8,2);
        mm = original_date.mid(5,2);
        yyyy = original_date.mid(0,4);

        result = yyyy + QString(".") + mm + QString((".")) + dd;

        delete []buf;
    }


   return result;
}

int ImageCompare(const QString &file1,const QString &file2)
{
    Mat mat1 = imread(file1.toStdString(),0);
    Mat mat2 = imread(file2.toStdString(),0);

    if(!QFileInfo::exists(file1))
        return FILE_DOES_NOT_EXIST;
    if(!QFileInfo::exists(file2))
        return FILE_DOES_NOT_EXIST;

    if(mat1.empty()||mat2.empty())
        return FILE_FORMAT_INCORRECT;

    if(mat1.size==mat2.size)
        return FILE_SAME;

    return FILE_NOT_SAME;
}

int MoveCompare(const QString &file1, const QString &file2)
{
    if(GetFileLength(file1)==GetFileLength(file2))
        return FILE_SAME;

    return FILE_NOT_SAME;
}

long long GetFileLength(const QString &file)
{
    FILE *fp = fopen(file.toStdString().c_str(), "rb");
    if(fp!=nullptr)
    {
        fseek(fp, 0, SEEK_END);
        return ftell(fp);
        fclose(fp);
    }

    return 0;
}

bool IsImageFile(const QFileInfo &fi)
{
    if((fi.suffix().toLower()=="jpeg")||
            (fi.suffix().toLower()=="jpg")||
            (fi.suffix().toLower()=="png")||
            (fi.suffix().toLower()=="tiff")||
            (fi.suffix().toLower()=="gif")||
            (fi.suffix().toLower()=="bmp"))
        return true;

    return false;
}

bool IsMoveFile(const QFileInfo &fi)
{
    if((fi.suffix().toLower()=="mov")||(fi.suffix().toLower()=="mp4"))
        return true;

    return false;
}

unsigned char *GenerateImageMD5(const QString &file)
{
    Mat mat = imread(file.toStdString(),0);

    if(mat.empty())
        return nullptr;

    md5_byte_t *digest = new md5_byte_t[16];
    md5_state_t state;

    md5_init(&state);
    md5_append(&state,mat.data,mat.total()*mat.elemSize());
    md5_finish(&state,digest);

    return digest;
}
