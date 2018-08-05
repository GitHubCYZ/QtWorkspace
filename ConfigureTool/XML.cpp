#include "XML.h"
#include <QDir>
#include <QFile>
#include <QDomDocument>
#include <QDebug>

XML::XML(QObject *parent) : QObject(parent)
{

}

bool XML::getAllRaderConfigDirPath(QString strConfigPath)
{
    QDir dir(strConfigPath);
    if( false == dir.exists() )
    {
        qDebug()<<"The path of Radar configuration list is not exit !";
        return false;
    }
    //只读取当前路径下的文件目录
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);

    QFileInfoList  fileInfoList = dir.entryInfoList();
    int nEntryCnt = fileInfoList.count();
    for( int i=0; i<nEntryCnt; ++i )
    {
        QFileInfo fileInfo = fileInfoList.at(i);
        if( fileInfo.isDir() ){
            m_raderList.append(fileInfo.absoluteFilePath());
            qDebug()<<"RootDirs:"<<fileInfo.baseName();
        }
    }
    return true;
}

bool XML::parseConfigurationFiles()
{
    int nRaderSz = m_raderList.count();
    for( int i=0; i<nRaderSz; ++i )
    {
        QString strEquipListFile("");//设备汇总文件名称
        QString strCfgDirPath = m_raderList.at(i);
        if( false == getEquipmentListFile(strCfgDirPath,strEquipListFile) ){
            qDebug()<<"get Equipment List File failed !";
            continue;
        }

        QSet<QString> readyLoadFiles;//预加载的配置文件名称集合
        if( 0 != readEquipmentList(strEquipListFile,readyLoadFiles) ){
            qDebug()<<"read Equipment List failed !";
            continue;
        }

        if( 0 != readAllEquipmentInfo(strCfgDirPath,readyLoadFiles) )
            return false;
    }
    return true;
}

bool XML::getEquipmentListFile(QString strPath,QString &strEquipListFile)
{
    QDir dir(strPath);
    if( false == dir.exists() )
    {
        qDebug()<<"The path of Radar configuration list is not exit !";
        return false;
    }
    //读取分机设备列表
    dir.setFilter(QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks);
    QFileInfoList  fileInfoList = dir.entryInfoList();
    int nEntrySz = fileInfoList.count();
    if( 1 == nEntrySz ){
        strEquipListFile = fileInfoList.at(0).absoluteFilePath();
    }
    else{
        qDebug()<<"There exist invalade file !";
        return false;
    }
    return true;
}

int XML::readEquipmentList(QString strPath,QSet<QString> &readyLoadFiles)
{
    QFile file(strPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Open XML Failed,FilePath:"<<strPath;
        return -2;
    }

    QDomDocument doc;
    QString error = "";
    int row = 0, column = 0;
    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return -1;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if( "RaderEquipment"==root.tagName() && "true"==root.attribute("enable") )
    {
        qDebug()<<"Device:"<<root.attribute("name");
        //emit createRaderItem(root.attribute("name"));
        QDomNode node = root.firstChild();
        while(!node.isNull()) {
           QDomElement element = node.toElement(); // try to convert the node to an element.
           if(!element.isNull()) {
               if( "SubEquipment" == element.tagName() && "true" == element.attribute("enable")){
                  QString strFileName = element.attribute("name");
                  readyLoadFiles.insert(strFileName);
              }
           }
           node = node.nextSibling();
        }
    }
    return 0;
}

int XML::readAllEquipmentInfo(QString strPath,QSet<QString> readyLoadFiles)
{
    QDir dir(strPath);
    if( false == dir.exists() )
    {
        qDebug()<<"The path of Radar configuration list is not exit !";
        return -2;
    }
    //读取分机设备列表
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);
    QFileInfoList  fileInfoList = dir.entryInfoList();
    int nEntrySz = fileInfoList.count();
    if( 1 == nEntrySz ){
        QFileInfo fileInfo = fileInfoList.at(0);
        if( fileInfo.isDir() )
        {
            QString strSubDir(fileInfo.absoluteFilePath());
            QDir subDir(strSubDir);
            if( false == subDir.exists() )
            {
                qDebug()<<"The path of Radar configuration subdir is not exit !";
                return -1;
            }
            //读取分机设备列表
            subDir.setFilter(QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks);
            QFileInfoList  subFileInfoList = subDir.entryInfoList();
            int nSubSz = subFileInfoList.size();
            for( int j=0;j<nSubSz; ++j )
            {
                QFileInfo subFileInfo = subFileInfoList.at(j);
                if(readyLoadFiles.find(subFileInfo.baseName()) != readyLoadFiles.end()){
                    readEquipmentInfo(subFileInfo.absoluteFilePath());
                    qDebug()<<"BaseName:"<<subFileInfo.baseName();
                }
            }
        }
    }
    return 0;
}

int XML::readEquipmentInfo(QString strPath)
{
    QFile file(strPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Open XML Failed,FilePath:"<<strPath;
        return -2;
    }

    QDomDocument doc;
    QString error = "";
    int row = 0, column = 0;
    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return -1;
    }
    file.close();

    RaderInfo raderInfo;
    raderInfo.filePath = strPath;
    //Root
    QDomElement root = doc.documentElement();
    if( !root.isNull() )
    {
        if( "subjectType" == root.tagName() )
        {
            raderInfo.isComposite = root.attribute("isComposite");
            raderInfo.name = root.attribute("name");
            raderInfo.displayName = root.attribute("displayName");
            raderInfo.category = root.attribute("category");
            raderInfo.version = root.attribute("version");
            raderInfo.icon = root.attribute("icon");

            //Second Level
            //raderInfo.secLevel.description
            QDomNode secNode = root.firstChildElement();
            raderInfo.secLevel.description = secNode.toElement().text();
            qDebug()<<raderInfo.secLevel.description;

            //subjectInfoMD
            secNode = secNode.nextSibling();
            if( !secNode.isNull() ){
                QDomElement secElem = secNode.toElement();
                raderInfo.secLevel.name = secElem.attribute("name");
                raderInfo.secLevel.displayName = secElem.attribute("displayName");
                raderInfo.secLevel.type = secElem.attribute("type");

                //Third Level
                QDomNode thirdNode = secElem.firstChild();
                while (!thirdNode.isNull()) {
                   QDomElement thirdElem = thirdNode.toElement(); // try to convert the node to an element.
                   if(!thirdElem.isNull()) {
                        Third_Level thirdLevel;
                        thirdLevel.source = thirdElem.attribute("source");
                        thirdLevel.readOnly = thirdElem.attribute("readOnly");
                        thirdLevel.name = thirdElem.attribute("name");
                        thirdLevel.displayName = thirdElem.attribute("displayName");
                        thirdLevel.type = thirdElem.attribute("type");
                        //raderInfo.secLevel.ents.push_back(thirdLevel);

                        //Forth Level
                        QDomNode forthNode = thirdElem.firstChild();
                        while (!forthNode.isNull()) {
                            QDomElement forthElem = forthNode.toElement(); // try to convert the node to an element.
                            if(!forthElem.isNull()) {
                                Forth_Level forthLevel;
                                forthLevel.source = forthElem.attribute("source");
                                forthLevel.name = forthElem.attribute("name");
                                forthLevel.displayName = forthElem.attribute("displayName");
                                forthLevel.dataType = forthElem.attribute("dataType");
                                forthLevel.readWriteFlag = forthElem.attribute("readWriteFlag");
                                forthLevel.validator = forthElem.attribute("validator");
                                forthLevel.displayType = forthElem.attribute("displayType");
                                forthLevel.unit = forthElem.attribute("unit");
                                forthLevel.paramType = forthElem.attribute("paramType");
                                forthLevel.hidden = forthElem.attribute("hidden");
                                forthLevel.macroConfig = forthElem.attribute("macroConfig");
                                thirdLevel.attrs.push_back(forthLevel);
                            }
                            forthNode = forthNode.nextSibling();
                        }
                        raderInfo.secLevel.ents.push_back(thirdLevel);
                   }
                   thirdNode = thirdNode.nextSibling();
                }
            }
        }
    }
    m_raderInfoList.push_back(raderInfo);
    return 0;
}
