#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <iostream>
#include "RelighterController.h"
#include "..\RawDataReader\RawDataReader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if(args.size() < 2)
    {
        std::cout << ">>> KESM Relighter Controller ver " << kRelighterControllerVersion << " <<<" << std::endl;
        std::cout << "Relight images on Cropped folders." << std::endl;

        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " DataRootFolder ColumnNumber" << std::endl;
        return kKESM_ERROR;
    }
    ///////////////////////////
    // TEST TEST TEST
    QString relighterProcessName = kRELIGHTER;

    ////////////////////////////////////////////
    // Detector Controller
    KESM::RelighterController relighter;

    ///////////////////////////////////
    // test data - will be given args
    //    QString dataRootFolder = "K:/Projects/KESMData/2008-01-23 Whole mouse brain vasculature";
    //    QString destDataRootFolder = "K:/Projects/KESMData";
    //    QString columnNum = "0";
    //relighter.args.sourceDataFolder = args[1];
    QString dataRootFolder = args[1];
    int columnNumber = args[2].toInt();
    //relighter.args.columnNumber = args[2];

    ///////////////////////////////////////////////////////////////////
    // attach '/' at the end of the folder name when the user forgets.
    if(dataRootFolder.endsWith("/") == false)
        dataRootFolder.append("/");

    ///////////////////////////////////////////////
    // The name of an XML containing raw data info
    //relighter.args.xmlFilename = QString("%1%2").arg(relighter.args.destinationDataFolder).arg(kRAW_DATA_INFO_FILENAME);
    QString xmlFilename = QString("%1%2").arg(dataRootFolder).arg(kRAW_DATA_INFO_FILENAME);
    KESM::RawDataReader reader;
    reader.readFile(xmlFilename);

    // create Relighted folder if there is not
    QString relightedFolder = QString("%1%2").arg(dataRootFolder).arg(kRELIGHTED_FOLDER_NAME);
    QDir relightedDir(relightedFolder);
    if(relightedDir.exists() == false)
        relightedDir.mkdir(relightedFolder);

    ////////////////////////////////////////////////
    // Make a list of all images in a column folder.
    QString croppedDataFolderNameWithColumnNumber = QString("%1%2/%3").arg(dataRootFolder).arg(kCROPPED_FOLDER_NAME).arg(columnNumber);
    QString relightedDataFolderNameWithColumnNumber = QString("%1%2/%3").arg(dataRootFolder).arg(kRELIGHTED_FOLDER_NAME).arg(columnNumber);
    QDir croppedDataDir(croppedDataFolderNameWithColumnNumber);
    QStringList filters;
    filters << QString("*%1").arg(kSOURCE_IMAGE_FILE_EXT);
    croppedDataDir.setNameFilters(filters);
    QStringList imageFileList = croppedDataDir.entryList(QDir::Files);

    // create Relighted folder if there is not
    QDir relightedWithColumnNumberDir(relightedDataFolderNameWithColumnNumber);
    if(relightedWithColumnNumberDir.exists() == false)
        relightedWithColumnNumberDir.mkdir(relightedDataFolderNameWithColumnNumber);

    std::cout << ">>> KESM Relighter Controller ver " << kRelighterControllerVersion << " <<<" << std::endl;
    std::cout << "Relight images." << std::endl;
    std::cout << "dataRootFolder: " << qPrintable(dataRootFolder) << std::endl;
    std::cout << "Col: " << columnNumber << std::endl;
    std::cout << imageFileList.count() << " files" << std::endl;

    for (int i = 0; i < imageFileList.count(); i++) {
        QString inputImageFilename = QString("%1/%2").arg(croppedDataFolderNameWithColumnNumber).arg(imageFileList[i]);
        QString outputImageFilename = QString("%1/%2").arg(relightedDataFolderNameWithColumnNumber).arg(imageFileList[i]);

        QStringList relighterArgList;
        relighterArgList << inputImageFilename << outputImageFilename;

        relighter.process.start(relighterProcessName, relighterArgList);
        relighter.process.waitForFinished();

        if(relighter.isProcessError)
            break;
    }

    return kKESM_OK; //a.exec();

}
