#include <QtCore/QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <iostream>
#include "../include/kesm.h"
#include "RawDataReader.h"
#include "TissueAreaWriter.h"
#include "TissueAreaDetectorController.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if(args.size() < 3)
    {
        std::cout << ">>> KESM Tissue Area Dectector Controller ver 1.0 <<<" << std::endl;
        std::cout << "Detects the tissue areas of all KESM raw images in a column." << std::endl;

        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " SrcRawDataFolder ColumnNumber DstRootDataFolder" << std::endl;
        return kKESM_ERROR;
    }
    ///////////////////////////
    // TEST TEST TEST
    QString detectorProcessName = kTISSUE_AREA_DETECTOR;

    ////////////////////////////////////////////
    // Detector Controller
    KESM::TissueAreaDetectorController detector;

    ///////////////////////////////////
    // test data - will be given args
    //    QString srcDataFolder = "K:/KESMRawData/2008-01-23 Whole mouse brain vasculature";
    //    QString destDataRootFolder = "K:/Projects/KESMData";
    //    QString columnNum = "0";
    detector.args.sourceDataFolder = args[1];
    QString destDataRootFolder = args[2];
    detector.args.columnNumber = args[3];

    ///////////////////////////////////////////////////////////////////
    // attach '/' at the end of the folder name when the user forgets.
    if(detector.args.sourceDataFolder.endsWith("/") == false)
        detector.args.sourceDataFolder.append("/");

    ///////////////////////////////////////
    // Get the source folder name not including full path
    detector.args.rawDataName = detector.args.sourceDataFolder.section('/', -2);
    detector.args.rawDataName.chop(1);

    if(detector.args.destinationDataFolder.endsWith("/"))
        detector.args.destinationDataFolder = detector.args.destinationDataFolder.append(detector.args.rawDataName);
    else
        detector.args.destinationDataFolder = QString("%1/%2/").arg(destDataRootFolder).arg(detector.args.rawDataName);

    ///////////////////////////////////////////////
    // The name of an XML containing raw data info
    detector.args.xmlFilename = QString("%1%2").arg(detector.args.destinationDataFolder).arg(kRAW_DATA_INFO_FILENAME);

    KESM::RawDataReader reader;
    reader.readFile(detector.args.xmlFilename);

    /////////////////
    // Column folder name
    QString formattedColumnNumber;
    formattedColumnNumber.sprintf("%0"kCOLUMN_DIGITS"d", detector.args.columnNumber.toInt());

    ////////////////////
    // Tempalte filename
    detector.args.templateFilename = QString("%1%2"kSOURCE_IMAGE_FILE_EXT).arg(detector.args.destinationDataFolder).arg(kTEMPLATE_FILENAME);

    ////////////////////////////////////////////////
    // Make a list of all images in a column folder.
    QString rawDataFolderNameWithColumnNumber = QString("%1%2").arg(detector.args.sourceDataFolder).arg(formattedColumnNumber);
    QDir rawDataDir(rawDataFolderNameWithColumnNumber);
    QStringList filters;
    filters << QString("*%1").arg(kSOURCE_IMAGE_FILE_EXT);
    rawDataDir.setNameFilters(filters);
    QStringList imageFileList = rawDataDir.entryList(QDir::Files);
    QString imageFilename;

    std::cout << ">>> KESM Tissue Area Dectector Controller ver 1.0 <<<" << std::endl;
    std::cout << "Detects the tissue areas of all KESM raw images in a column." << std::endl;
    std::cout << "Src: " << qPrintable(detector.args.sourceDataFolder) << std::endl;
    std::cout << "Dst: " << qPrintable(detector.args.destinationDataFolder) << std::endl;
    std::cout << "Col: " << qPrintable(detector.args.columnNumber) << std::endl;
    std::cout << imageFileList.count() << " files" << std::endl;

    ////////////////////////
    // Get Tissue Area Writer ready
    KESM::TissueAreaWriter::Column column;
    column.id = detector.args.columnNumber.toInt();
    column.width = reader.rawData.tissueWidth;

    KESM::TissueAreaWriter writer(column);
    KESM::TissueAreaWriter::RawImage rawImage;
    QList<int> list;

    for (int i = 0; i < imageFileList.count(); i++) {
        imageFilename = QString("%1/%2").arg(rawDataFolderNameWithColumnNumber).arg(imageFileList[i]);

        QStringList detectorArgList;
        detectorArgList << QString::number(reader.rawData.tissueWidth) << detector.args.templateFilename << imageFilename << "s";

        detector.process.start(detectorProcessName, detectorArgList);
        detector.process.waitForFinished();

        if(detector.isProcessError)
            break;

        //QString output = QString("startX is %1").arg(detector.startX);
        std::cout << i+1 << "/" << imageFileList.count() << ": " << qPrintable(imageFileList[i]) << ": " << detector.startX << std::endl;

        rawImage.name = imageFileList[i];
        rawImage.startX = detector.startX;
//        rawImage.orgStartX = detector.startX;
//        rawImage.adjust = false;
        rawImage.valid = detector.startX == kKESM_ERROR ? false : true;

        list.append(detector.startX);
        writer.getColumn()->images.append(rawImage);
    }

    // Let's make a separate process to remove outliers
    // This can be more complicated than it looks.

//    //////////////////////////////
//    // Get a chunk point
//    QVector<int> chunks;
//    qSort(list.begin(), list.end());
//    for(int i = 1; i < list.count(); i++) {
//        if((list[i-1] > list[i]+kRIGHT_EDGE_WINDOW_SIZE/2) || (list[i-1] < list[i]-kRIGHT_EDGE_WINDOW_SIZE/2)) {
//            chunks.append(list[i-1]);
//            std::cout << "Chuck: " << list[i-1] << std::endl;
//        }
//    }

    //////////////////////////////
    // Remove outliers
    // 1. get median value of startx
    // 2. if startx is smaller of greater than the median by a threshold width
    //    set startx with the median.
    //qSort(list.begin(), list.end());
//    int medianStartX = list[list.size()/2];
//    for(int i = 0; i < writer.getColumn()->images.count(); i++) {
//        if((writer.getColumn()->images[i].startX > (medianStartX + kRIGHT_EDGE_WINDOW_SIZE/2))
//            || (writer.getColumn()->images[i].startX < (medianStartX - kRIGHT_EDGE_WINDOW_SIZE/2))) {
//            std::cout << "Fixed " << qPrintable(imageFileList[i]) << ":" << writer.getColumn()->images[i].startX << "->" << medianStartX << std::endl;
//            writer.getColumn()->images[i].startX = medianStartX;
//            writer.getColumn()->images[i].adjust = true;
//        }
//    }

    QString croppedFolder = QString("%1%2").arg(detector.args.destinationDataFolder).arg(kCROPPED_FOLDER_NAME);
    QDir croppedDir(croppedFolder);
    if(croppedDir.exists() == false)
        croppedDir.mkdir(croppedFolder);

    QString tissueAreaXmlFilename = QString("%1/Raw_%2.xml").arg(croppedFolder).arg(detector.args.columnNumber);
    writer.writeFile(tissueAreaXmlFilename);
    QString tissueAreaCSVFilename = QString("%1/Raw_%2.csv").arg(croppedFolder).arg(detector.args.columnNumber);
    writer.writeCSVFile(tissueAreaCSVFilename);

    return kKESM_OK; //a.exec();
}
