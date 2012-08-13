#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <iostream>
#include "MergerController.h"

class ProcessInfo {
public:
    ProcessInfo() {}
    bool init(QString dataRootFolder);
    bool prepareArgs(QString imageInfoFileName); /* , QString mergedImageFileName) */

public:
    QString processName;
    // ------
    QString dataRootFolder;
    // arguments
    struct Args {
        QString imageInfoFilePath;
        QString mergedImageFilePath;
        bool invertImage;
    };
    Args args;
};

bool ProcessInfo::init(QString dataRootFolder)
{
    processName = kMergerProcessName;
    QString folder = QDir::fromNativeSeparators(dataRootFolder);
    if(folder.endsWith('/') == false)
        folder.append("/");
    this->dataRootFolder = folder;

    ///////////////////////////////////////
    // create the destination folder if there is not
    QString destFolder = QString("%1%2").arg(this->dataRootFolder).arg(kMERGED_FOLDER_NAME);
    QDir dir(destFolder);
    if(dir.exists() == false)
        dir.mkdir(destFolder);

    return true;
}

bool ProcessInfo::prepareArgs(QString imageInfoFileName) /* , QString mergedImageFileName) */
{
    args.imageInfoFilePath = QString("%1%2/%3").arg(dataRootFolder).arg(kMergerInfoFolderName).arg(imageInfoFileName);
    int pos = imageInfoFileName.lastIndexOf(".txt");
    if(pos == -1)
        return false;

    QString mergedImageFileName = imageInfoFileName.left(pos);
    mergedImageFileName.append(kSOURCE_IMAGE_FILE_EXT);

    args.mergedImageFilePath = QString("%1%2/%3").arg(dataRootFolder).arg(kMERGED_FOLDER_NAME).arg(mergedImageFileName);

    ////////////////////////////
    // invert images by default
    // !! default value for Merger: remain origianal
    // !! default for MergerController: invert intensity for visualization
    args.invertImage = true;

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if(args.size() < 2)
    {
        std::cout << ">>> KESM Merger Controller ver " << kMergerControllerVersion << " <<<" << std::endl;
        std::cout << "Merge columnized images into one image stack." << std::endl;

        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " DataRootFolder" << std::endl;
        return kKESM_ERROR;
    }

    ProcessInfo processInfo;
    processInfo.init(args[1]);

    ////////////////////////////////////////////////
    // Make a list of all images in the folder.
    QString imageInfoFolderName = QString("%1%2/").arg(processInfo.dataRootFolder).arg(kMergerInfoFolderName);
    QDir dir(imageInfoFolderName);
    QStringList filters;
    filters << QString("i*%1").arg(".txt");
    dir.setNameFilters(filters);
    QStringList imageInfoFileList = dir.entryList(QDir::Files);

    std::cout << ">>> KESM Merger Controller ver " << kRelighterControllerVersion << " <<<" << std::endl;
    std::cout << "Merge images." << std::endl;
    std::cout << "dataRootFolder: " << qPrintable(processInfo.dataRootFolder) << std::endl;
    std::cout << imageInfoFileList.count() << " file(s) will be processed" << std::endl;
    std::cout << "invertImage:" << (processInfo.args.invertImage ? "yes" : "no") << std::endl;

    ////////////////////////////////////////////
    // Controller
    KESM::MergerController controller;

    for (int i = 0; i < imageInfoFileList.count(); i++) {
        processInfo.prepareArgs(imageInfoFileList[i]);

        QStringList argList;
        QString invertFlag;
        if(processInfo.args.invertImage)
        {
            invertFlag = kInvertImageFlag"on";
            argList << processInfo.args.imageInfoFilePath << processInfo.args.mergedImageFilePath << invertFlag;
        }
        else
            argList << processInfo.args.imageInfoFilePath << processInfo.args.mergedImageFilePath;

        std::cout << "processing... " << i << ": " << qPrintable(imageInfoFileList[i]) << std::endl;

        controller.process.start(processInfo.processName, argList);
        controller.process.waitForFinished();

        if(controller.isProcessError)
            break;
    }

    return kKESM_OK; //a.exec();

}
