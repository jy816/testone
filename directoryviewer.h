#ifndef DIRECTORYVIEWER_H
#define DIRECTORYVIEWER_H

#include <QtGui/QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QMenu>

class DirectoryViewer : public QWidget
{
    Q_OBJECT

public:
    DirectoryViewer(QWidget *parent = 0);
    ~DirectoryViewer();
    QString sFilePath;
    QString DfileName;
    QString RFilepath;
    qint32 flag;
    bool  copyFileToPath(QString sourceDir,QString toDir,QString filename,bool coverFileIfExist);
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
    bool coverFileIfExist;

public slots:
    QString copy_action();
    bool paste_action();
    void createDirectory();
    void createFile();
    void remove_action();

protected:

    QMenu  *cmenu;


private slots:
    void show_contextmenu(const QPoint&);

private:
    QFileSystemModel *model;
    QTreeView *treeView;
};

#endif
