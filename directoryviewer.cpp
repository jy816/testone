#include "directoryviewer.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QWidget>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

DirectoryViewer::DirectoryViewer(QWidget *parent)
    : QWidget(parent)
{
   // QFile file("/home/troy/yangbin/data.txt");
    model = new QFileSystemModel;
    model->setReadOnly(false);            //���ÿ����޸�
    model->setRootPath(QDir::currentPath());

    treeView = new QTreeView;
    treeView->setModel(model);

    treeView->header()->setStretchLastSection(true);
    treeView->header()->setSortIndicator(0, Qt::AscendingOrder);
    treeView->header()->setSortIndicatorShown(true);
    treeView->header()->setClickable(true);
    treeView->header()->setResizeMode(QHeaderView::ResizeToContents);


    QModelIndex index = model->index("/");
    treeView->expand(index);      //��ǰ��չ��
    treeView->scrollTo(index);    //��λ����ǰ��
    treeView->resizeColumnToContents(0);

    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView, SIGNAL(customContextMenuRequested(const QPoint&)),  this, SLOT(show_contextmenu(const QPoint&)));


    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(treeView);

    setLayout(vlayout);
}

DirectoryViewer::~DirectoryViewer()
{
}


void DirectoryViewer::show_contextmenu(const QPoint& pos)
{
       QMenu* cmenu = new QMenu(treeView);


       QAction *CopyAction = cmenu->addAction(tr("����"));
//       CopyAction->setShortcut(QKeySequence(tr("Ctrl+C")));
//       CopyAction->setStatusTip("Copy File");
       cmenu->addSeparator();
       QAction *PasteAction = cmenu->addAction(tr("ճ��"));
//       PasteAction->setShortcut(tr("Ctrl+V"));
//       PasteAction->setStatusTip("Paste File");
       cmenu->addSeparator();
       QAction *RemoveAction = cmenu->addAction(tr("ɾ��"));
//       RemoveAction->setShortcut(tr("Ctrl+d"));
//       RemoveAction->setStatusTip("Delete File or Directory");
       cmenu->addSeparator();
   // QAction *CreateAction = cmenu->addAction(tr("�½�"));
     QMenu* qmenu = cmenu->addMenu(tr("�½�"));
     QAction *CreateDirAction = qmenu->addAction(tr("�½�Ŀ¼"));
     QAction *CreateFileAction = qmenu->addAction(tr("�½��ı��ļ�"));

       connect(CopyAction, SIGNAL(triggered(bool)), this, SLOT(copy_action()));
       connect(PasteAction, SIGNAL(triggered(bool)), this, SLOT(paste_action()));
       connect(RemoveAction, SIGNAL(triggered(bool)), this, SLOT(remove_action()));
       connect(CreateDirAction, SIGNAL(triggered(bool)), this, SLOT(createDirectory()));
       connect(CreateFileAction, SIGNAL(triggered(bool)), this, SLOT(createFile()));

       cmenu->exec(QCursor::pos());//�ڵ�ǰ���λ����ʾ
}

void DirectoryViewer::createDirectory()
{
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid())
    {
        return;
    }
    QString dirName = QInputDialog::getText(this, tr("Create Directory"), tr("Directory name"));
    if (!dirName.isEmpty())
    {
        if (!model->mkdir(index, dirName).isValid())
        {
            QMessageBox::information(this, tr("Create Directory"), tr("�½�Ŀ¼ʧ��"));
        }
    }
}

void DirectoryViewer::createFile()
{
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid())
    {
        return;
    }
    if (model->fileInfo(index).isDir())
    {
        QString sFilepath = model->filePath (index);
        QDir DFilePath=sFilepath;
        QString dirName = QInputDialog::getText(this, tr("Create File"), tr("File name"));
        if (!dirName.isEmpty())
        {
        QFileInfoList fileInfoList = DFilePath.entryInfoList();
        foreach(QFileInfo fileInfo, fileInfoList)
        {

            if(dirName==fileInfo.fileName()){
                QMessageBox::information(this, tr("ERROR��"), tr("�ļ��Ѿ����ڣ��Ƿ��滻"));
                if(QMessageBox::Ok)
                    {
                     DFilePath.remove(fileInfo.fileName());
                     QFile myfile(sFilepath+"/"+dirName);
                     myfile.open(QIODevice::ReadWrite);
                      myfile.close();
                      return;
                }
                else if(QMessageBox::Cancel)
                    {
                    return;
                }
            }

            QFile myfile(sFilepath+"/"+dirName);
            myfile.open(QIODevice::ReadWrite);
             myfile.close();
             return;
            }
    }
    return;
    }

}

void DirectoryViewer::remove_action()
{
    QModelIndex index = treeView->currentIndex();
    qDebug()<<"0";
    if (!index.isValid())
    {
        return;
    }    //bool ok;
    if (model->fileInfo(index).isDir())
    {
        //qDebug()<<"1";
        QString RFilepath = model->filePath (index);
       // QString dirName=model->fileName(index);
       qDebug()<<RFilepath;
          QDir dir(RFilepath);
          QString tmpdir = "";
          if(!dir.exists()){
            return;        }
          QFileInfoList fileInfoList = dir.entryInfoList();
          foreach(QFileInfo fileInfo, fileInfoList){
            if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
              continue;
            if(fileInfo.isDir()){
              tmpdir = RFilepath  + ("/") + fileInfo.fileName();
              RFilepath=tmpdir;
             // qDebug()<<"4";
              remove_action();
              dir.rmdir(fileInfo.fileName()); /**< �Ƴ���Ŀ¼ */
            }
            else if(fileInfo.isFile()){
              QFile tmpFile(fileInfo.fileName());
              dir.remove(tmpFile.fileName()); /**< ɾ����ʱ�ļ� */
            }
            else{ ; }
          }
          dir.cdUp();            /**< �����ϼ�Ŀ¼����Ϊֻ�з����ϼ�Ŀ¼���ſ���ɾ�����Ŀ¼ */
          if(dir.exists(RFilepath)){
            if(!dir.rmdir(RFilepath))
              return;
          }
    }
    else
    {
         //qDebug()<<"2";
         model->remove(index);
    }
}

 QString DirectoryViewer::copy_action()
{
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid())
    {
        return 0;
    }
    if (model->fileInfo(index).isDir())
    {
         QString sFilepath = model->filePath (index);
         //QString filename=model->fileName(sFilepath);
         QFileInfo fi( sFilepath  );
         QString filename = fi.fileName();
         DfileName=filename;
         sFilePath=sFilepath;
         flag=1;
         return sFilePath;
    }
    else
    {
       QString sFilepath = model->filePath (index);
//        QString filename=model->fileName(index);
       QFileInfo fi( sFilepath  );
       QString filename = fi.fileName();
       qDebug()<<"kuaichegn";
       DfileName=filename;
       sFilePath=sFilepath;
       flag=0;
       return sFilePath;
    }
}

 bool DirectoryViewer::paste_action()
{
     QModelIndex index = treeView->currentIndex();
     if (!index.isValid())
     {
         return false;
     }
     if (model->fileInfo(index).isDir()&&flag==0)
     {
         QString DsFilePath = model->filePath (index);
         coverFileIfExist=1;
         DirectoryViewer::copyFileToPath(sFilePath,DsFilePath,DfileName,coverFileIfExist);
         return true;
     }
     else if(model->fileInfo(index).isDir()&&flag==1)
     {
         QString DsFilePath = model->filePath (index);
         coverFileIfExist=1;
         DirectoryViewer::copyDirectoryFiles(sFilePath,DsFilePath,coverFileIfExist);
          return true;
     }
     return true;
}

bool DirectoryViewer::copyFileToPath(QString sourceDir,QString toDir,QString filename,bool coverFileIfExist)
{
        toDir=toDir+"/"+filename;
        qDebug()<<toDir;
        if (sourceDir == toDir){
            return true;

        }
        if (!QFile::exists(sourceDir)){
            return false;
        }
        QDir *createfile     = new QDir;
        bool exist = createfile->exists(toDir);
        if (exist){
            if(!coverFileIfExist){
                createfile->remove(toDir);
            }
        }
        QFile a(sourceDir);
        a.copy(toDir);
        return true;
}
bool DirectoryViewer::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir = fromDir;
    QDir targetDir =toDir+"/"+DfileName;
    qDebug()<<targetDir;
      if(!targetDir.exists()){    /**< ���Ŀ��Ŀ¼�����ڣ�����д��� */
        if(!targetDir.mkdir(targetDir.absolutePath()))
          return false;
      }

      QFileInfoList fileInfoList = sourceDir.entryInfoList();
      foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
          continue;

        if(fileInfo.isDir()){    /**< ��ΪĿ¼ʱ���ݹ�Ľ���copy */
          if(!copyDirectoryFiles(fileInfo.filePath(),
                 targetDir.filePath(fileInfo.fileName()),
                 coverFileIfExist))
        return false;
        }
        else{            /**< �������ǲ���ʱ�������ļ�����ɾ������ */
          if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
        targetDir.remove(fileInfo.fileName());
          }

          /// �����ļ�copy
          if(!QFile::copy(fileInfo.filePath(),
             targetDir.filePath(fileInfo.fileName()))){
        return false;
          }
        }
      }
      return true;
}
