#include <QDir>
#include <QDateTime>
#include <QProgressBar>
#include <QHashIterator>
#include "utility.h"
#include "qfilecopythread.h"

QFileCopyThread::QFileCopyThread()
{
    progress_bar = NULL;
    run_flag = false;
}

QFileCopyThread::~QFileCopyThread()
{

}

void QFileCopyThread::SetSrcDir(const QString &folder)
{
    src_folder = folder;
    src_folder_files.clear();
    GetFolderImages(src_folder,src_folder_files,folder_tree);
}

void QFileCopyThread::SetDstDir(const QString &folder)
{
    dst_folder = folder;
}

void QFileCopyThread::SetProgressBar(QProgressBar *pb)
{
    progress_bar = pb;
}

QHash<QString,QStringList> QFileCopyThread::GetFileTree()
{
    return folder_tree;
}

QString QFileCopyThread::GetSrcDir() const
{
    return src_folder;
}

QString QFileCopyThread::GetDstDir() const
{
    return dst_folder;
}

QStringList QFileCopyThread::GetFileList() const
{
    return src_folder_files;
}

void QFileCopyThread::stop()
{
    run_flag = false;
}

 void QFileCopyThread::run()
 {
     if(src_folder_files.empty())
         return ;

     QFileInfo fileinfo;
     QString date_folder;
     QFile file;
     QDir copyto_dir;

     int i;
     if(progress_bar!=NULL)
     {
        progress_bar->setMinimum(0);
        progress_bar->setMaximum(src_folder_files.size());
     }

     run_flag = true;


//     for(i=0;i<src_folder_files.size()&&run_flag;++i)
//     {

//         fileinfo.setFile(src_folder_files[i]);
//         date_folder = GenerateFolderName(fileinfo);
//         copyto_dir.setPath(dst_folder+"/"+date_folder);

//         if(!copyto_dir.exists())
//             copyto_dir.mkdir(dst_folder+"/"+date_folder);

//         file.setFileName(src_folder_files[i]);
//         if(QFileInfo(dst_folder+"/"+date_folder+"/"+fileinfo.fileName()).exists())
//         {
//             if(IsImageFile(fileinfo))
//                 if(ImageCompare(src_folder_files[i],dst_folder+"/"+date_folder+"/"+fileinfo.fileName())!=FILE_SAME)
//                     file.copy(src_folder_files[i],dst_folder+"/"+date_folder+"/"+fileinfo.baseName()+QDateTime::currentDateTime().toString()+fileinfo.suffix());
//                 else
//                 {
//                     if(progress_bar!=NULL)
//                         progress_bar->setValue(i+1);
//                     continue;
//                 }

//             if(IsMoveFile(fileinfo))
//                 if(MoveCompare(src_folder_files[i],dst_folder+"/"+date_folder+"/"+fileinfo.fileName())!=FILE_SAME)
//                     file.copy(src_folder_files[i],dst_folder+"/"+date_folder+"/"+fileinfo.baseName()+QDateTime::currentDateTime().toString()+fileinfo.suffix());
//         }
//         else
//         {
//             file.copy(src_folder_files[i],dst_folder+"/"+date_folder+"/"+fileinfo.fileName());
//         }
// //        unsigned char* md5_str = GenerateImageMD5(src_folder_files[i]);
// //        char hex_output[16*2 + 1];
// //        memset(hex_output,0,sizeof(char)*32+1);
// //        for (int di = 0; di < 16; ++di)
// //            sprintf(hex_output + di * 2, "%02x", md5_str[di]);
// //        md5_str_label->setText(QString(hex_output));
// //        if(md5_str!=NULL)
// //            delete md5_str;
//        if(progress_bar!=NULL)
//            progress_bar->setValue(i+1);
//     }

         QHashIterator<QString, QStringList> hash_itor(folder_tree);
         int image_count = 1;
         while(hash_itor.hasNext())
         {
             hash_itor.next();
             QStringList str_list = hash_itor.value();
             date_folder = hash_itor.key();
             copyto_dir.setPath(dst_folder+"/"+date_folder);

             if(!copyto_dir.exists())
                 copyto_dir.mkdir(dst_folder+"/"+date_folder);
             for(int i=0;i<str_list.size();++i)
             {
                 fileinfo.setFile(str_list[i]);
                 file.setFileName(str_list[i]);
                 if(QFileInfo(dst_folder+"/"+date_folder+"/"+fileinfo.fileName()).exists())
                 {
                     if(IsImageFile(fileinfo))
                         if(ImageCompare(str_list[i],dst_folder+"/"+date_folder+"/"+fileinfo.fileName())!=FILE_SAME)
                             file.copy(str_list[i],dst_folder+"/"+date_folder+"/"+fileinfo.baseName()+QDateTime::currentDateTime().toString()+fileinfo.suffix());
                         else
                         {
                             if(progress_bar!=NULL)
                                 progress_bar->setValue(image_count);
                             ++image_count;
                             continue;
                         }

                     if(IsMoveFile(fileinfo))
                         if(MoveCompare(str_list[i],dst_folder+"/"+date_folder+"/"+fileinfo.fileName())!=FILE_SAME)
                             file.copy(str_list[i],dst_folder+"/"+date_folder+"/"+fileinfo.baseName()+QDateTime::currentDateTime().toString()+fileinfo.suffix());
                 }
                 else
                 {
                     file.copy(str_list[i],dst_folder+"/"+date_folder+"/"+fileinfo.fileName());
                 }
                 //        unsigned char* md5_str = GenerateImageMD5(src_folder_files[i]);
                 //        char hex_output[16*2 + 1];
                 //        memset(hex_output,0,sizeof(char)*32+1);
                 //        for (int di = 0; di < 16; ++di)
                 //            sprintf(hex_output + di * 2, "%02x", md5_str[di]);
                 //        md5_str_label->setText(QString(hex_output));
                 //        if(md5_str!=NULL)
                 //            delete md5_str;
                 if(progress_bar!=NULL)
                     progress_bar->setValue(image_count);

                 ++image_count;

             }
         }
 }
