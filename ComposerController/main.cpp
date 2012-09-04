#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <iostream>
#include "ComposerController.h"

class ProcessInfo {
public:
    ProcessInfo() {}
    bool init(QString dataPathName, int startIndex, int endIndex, int numberOfImages);
    bool prepareArgs(int index);
    QString getImageFileNameFromIndex(int index);

public:
    QString processName;
    // ------
    QString dataPathName;
    int startIndex;
    int endIndex;
    int numberOfImages;

    // arguments of the process
    struct Args {
        QString baseFilePathName;
        QString outputPath;
        int numberOfImages;
    };
    Args args;
};

QString ProcessInfo::getImageFileNameFromIndex(int index)
{
    // ------------
    // 1. find file starting with 'i<index number padding '0's.'
    QString imageFileNameFilter;
    QTextStream ts(&imageFileNameFilter);
    ts.setFieldWidth(KESM::indexedImagefileNameFormat.length());
    ts.setFieldAlignment(QTextStream::AlignRight);
    ts.setPadChar('0');
    ts << (index);

    QStringList filters;
    filters << QString("i%1*%2").arg(imageFileNameFilter).arg(kSOURCE_IMAGE_FILE_EXT);
    QDir baseDir(dataPathName);
    baseDir.setNameFilters(filters);
    QStringList imageFileList = baseDir.entryList(QDir::Files);
    if(imageFileList.size() == 0) // no file.. error...
        return false;
    QString imageFileName = imageFileList[0];
    QString imageFilePathName = QString("%1/%2").arg(dataPathName).arg(imageFileName);

    return imageFilePathName;
}

bool ProcessInfo::init(QString dataPathName, int startIndex, int endIndex, int numberOfImages)
{
    processName = kComposerProcessName;
    QString pathName = QDir::fromNativeSeparators(dataPathName);
    this->dataPathName = pathName;
    this->startIndex = startIndex;
    this->endIndex = endIndex;
    this->numberOfImages = numberOfImages;

    pathName = this->dataPathName.left(this->dataPathName.lastIndexOf('/')); // remove dir name from the dataPath
    args.outputPath = QString("%1/%2%3").arg(pathName).arg(kAttenuatedFolderName).arg(numberOfImages);
    args.numberOfImages = numberOfImages;

    ///////////////////////////////////////
    // create the destination folder if there is not
    QDir dir(args.outputPath);
    if(dir.exists() == false)
        dir.mkdir(args.outputPath);

    return true;
}

bool ProcessInfo::prepareArgs(int index)
{
    args.baseFilePathName = getImageFileNameFromIndex(index);
    // these two do not change according to index.. move to init.
    //    args.outputPath = dataPathName.left(dataPathName.indexOf('/')); // remove dir name from the dataPath
    //    args.numberOfImages = numberOfImages;

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if(args.size() < 5)
    {
        std::cout << ">>> KESM Composer Controller ver " << kComposerControllerVersion << " <<<" << std::endl;
        std::cout << "Control composers." << std::endl;

        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " dataPathName startIndex endIndex numberOfImagesToCompose" << std::endl;
        return kKESM_ERROR;
    }

    ProcessInfo processInfo;
    processInfo.init(args[1], args[2].toInt(), args[3].toInt(), args[4].toInt());

    std::cout << ">>> KESM Composer Controller ver " << kComposerControllerVersion << " <<<" << std::endl;
    std::cout << "Control composers." << std::endl;

    std::cout << "dataPathName:    " << qPrintable(processInfo.dataPathName) << std::endl;
    std::cout << "startIndex:      " << processInfo.startIndex << std::endl;
    std::cout << "endIndex:        " << processInfo.endIndex << std::endl;
    std::cout << "numberOfImages:  " << processInfo.numberOfImages << std::endl;

    ////////////////////////////////////////////
    // Controller
    KESM::ComposerController controller;

    for (int i = processInfo.startIndex; i < processInfo.endIndex; i+=kComposerUnitDepth) { //(processInfo.numberOfImages)/2) {
        processInfo.prepareArgs(i);

        QStringList argList;
        QString numberOfImages = QString("%1").arg(processInfo.args.numberOfImages);
        argList << processInfo.args.baseFilePathName << processInfo.args.outputPath << numberOfImages;

        std::cout << "processing... " << i << ": " << qPrintable(processInfo.args.baseFilePathName) << std::endl;

        controller.process.startDetached(processInfo.processName, argList);
        // default max wait time is 30 sec.
        // if more than 30 sec needed, we got to wait more...

        controller.process.waitForFinished();//kMaxProcessWaitTime);


        if(controller.isProcessError)
            break;
    }

    return kKESM_OK; //a.exec();

}
