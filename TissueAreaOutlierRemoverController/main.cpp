#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <iostream>
#include "TissueAreaOutlierRemoverController.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if(args.size() < 2)
    {
        std::cout << ">>> KESM TissueAreaOutlierRemover Controller ver " << kRelighterControllerVersion << " <<<" << std::endl;
        std::cout << "Control of tissue area outlier remover." << std::endl;

        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " DataRootFolder ColumnNumber" << std::endl;
        return kKESM_ERROR;
    }
    ///////////////////////////
    // TEST TEST TEST
    QString processName = kTISSUE_AREA_OUTLIER_REMOVER;

    ////////////////////////////////////////////
    // Detector Controller
    KESM::TissueAreaOutlierRemoverController controller;

    ///////////////////////////////////
    // test data - will be given args
    //    QString dataRootFolder = "K:/Projects/KESMData/2008-01-23 Whole mouse brain vasculature";
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
    //QString xmlFilename = QString("%1%2").arg(dataRootFolder).arg(kRAW_DATA_INFO_FILENAME);
    //KESM::RawDataReader reader;
    //reader.readFile(xmlFilename);

    // Check there is the Cropped folder
    QString croppedFolder = QString("%1%2").arg(dataRootFolder).arg(kCROPPED_FOLDER_NAME);
    QDir croppedDir(croppedFolder);
    if(croppedDir.exists() == false)
    {
        std::cout << "Cropped folder does not exist." << std::endl;
        return kKESM_ERROR;
    }

    std::cout << ">>> KESM TissueAreaOutlierRemover Controller ver " << kRelighterControllerVersion << " <<<" << std::endl;
    std::cout << "dataRootFolder: " << qPrintable(dataRootFolder) << std::endl;
    std::cout << "Col: " << columnNumber << std::endl;

    //if(columnNumber == -1)
    {
        QString rawXMLFileName = QString("%1/Raw_%2.xml").arg(croppedFolder).arg(columnNumber);
        QString newXMLFileName = QString("%1/%2.xml").arg(croppedFolder).arg(columnNumber);
        QString csvFileName = QString("%1/%2.csv").arg(croppedFolder).arg(columnNumber);
        QString chunkFileName = QString("%1/%2.txt").arg(croppedFolder).arg(columnNumber);

        QStringList argList;
        argList << rawXMLFileName << newXMLFileName << csvFileName << chunkFileName;

        controller.process.start(processName, argList);
        controller.process.waitForFinished();

        if(controller.isProcessError)
            return kKESM_ERROR;
    }

    return kKESM_OK; //a.exec();

}
