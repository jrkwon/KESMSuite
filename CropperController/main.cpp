#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <iostream>
#include "CropperController.h"
#include "RawDataReader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if(args.size() < 3)
    {
        std::cout << ">>> KESM Cropper Controller ver <<<" << kCropperControllerVersion << std::endl;
        std::cout << "Manages Cropper." << std::endl;

        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " SrcRawDataFolder DstRootDataFolder ColumnNumber" << std::endl;
        std::cout << "\t ColumnNumber: -1 for all columns" << std::endl;
        return kKESM_ERROR;
    }
    ///////////////////////////
    // TEST TEST TEST
    QString processorName = kCROPPER;

    //////////////////////////
    // make destination folder name & get a full path for kRAW_DATA_INFO_FILENAME

    QString sourceDataFolder = args[1];
    QString rootDestinationFolderName = args[2];
    int columnNumber = args[3].toInt();
    QString targetDataFolder; // this include the data name

    // attach '/' at the end of the folder name when the user forgets.
    if(sourceDataFolder.endsWith("/") == false)
        sourceDataFolder.append("/");

    // Extract the data name from the sourceDataFolder
    QString rawDataName = sourceDataFolder.section('/', -2);
    rawDataName.chop(1);

    if(rootDestinationFolderName.endsWith("/"))
        targetDataFolder = rootDestinationFolderName.append(rawDataName);
    else
        targetDataFolder = QString("%1/%2/").arg(rootDestinationFolderName).arg(rawDataName);

    ///////////////////////////////////////////////
    // The name of an XML containing raw data info
    QString xmlFilename = QString("%1%2").arg(targetDataFolder).arg(kRAW_DATA_INFO_FILENAME);
    KESM::RawDataReader reader;
    reader.readFile(xmlFilename);

//    if(columnNumber != -1) {
    // genterate folder names for source and destination
    QString columnString;
    columnString.sprintf("%0"kCOLUMN_DIGITS"d", columnNumber);
    QString sourceColumnFolderName = QString("%1%2").arg(sourceDataFolder).arg(columnString);
    QString destCroppedFolderName = QString("%1%2").arg(targetDataFolder).arg(kCROPPED_FOLDER_NAME);
    QString destCroppedColumnFolderName = QString("%1/%2").arg(destCroppedFolderName).arg(columnNumber);

    QDir dir(destCroppedColumnFolderName);
    if(dir.exists() == false)
    {
        if(dir.mkdir(destCroppedColumnFolderName) == false)
        {
            std::cout << "Fail to create the folder " << qPrintable(destCroppedColumnFolderName) << std::endl;
            return kKESM_ERROR;
        }
    }

    // generate a destination
    QString targetColumnFileName = QString("%1/%2.csv").arg(destCroppedFolderName).arg(columnNumber);

    ////////////////////////////////////////////
    // Cropper Controller
    KESM::CropperController cropper;

    // read the csv file to get filenames and their crop areas
    QFile inputFile(targetColumnFileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       // ignore the first line
       QString line = in.readLine();
       while ( !in.atEnd() )
       {
          line = in.readLine();
          QStringList items = line.split(",");
          // 0: filename, 1: original start x, 2: adjusted x
          QString sourceImagePath = QString("%1/%2").arg(sourceColumnFolderName).arg(items[0]);
          QString destCroppedImagePath = QString("%1/%2").arg(destCroppedColumnFolderName).arg(items[0]);

          // make sure that no space between ,
          QString args4Cropper = QString("%1,%2,0,%3,-1,%4").arg(sourceImagePath).arg(items[2]).arg(reader.rawData.tissueWidth).arg(destCroppedImagePath);
          //test QString args4Cropper = QString("%1,%2,0,%3,-1,%4").arg("K:/test  space/20080129_135927_x148.0967y23.6286z1.4710_t0.001000_v18.1940.jpg").arg(items[2]).arg(reader.rawData.tissueWidth).arg("K:/cropped.jpg");

          QStringList argList = args4Cropper.split(",");

          // trim in the file path if there is any white space before and after the file path
          for(QStringList::Iterator it = argList.begin(); it != argList.end(); it++)
              (*it) = (*it).trimmed();

          // execute cropper with the args
          cropper.process.start(processorName, argList);
          cropper.process.waitForFinished();

          if(cropper.isProcessError)
              break;
       }
    }
    inputFile.close();

        ///////////////////////////////////////////////
        // Generate args for Cropper
//    }
//    else { // all columns

//    }


    ////////////////////////////////////////////
    // Cropper Controller
/*    KESM::CropperController cropperController;
    cropperController.args.sourceDataFolder = sourceDataFolder;
    cropperController.args.destinationDataFolder = targetDataFolder;
    cropperController.args.rawDataName = rawDataName;
    cropperController.args.xmlFilename = xmlFilename;
*/
    //KESM::CropperController cropperController(sourceDataFolder, destinationDataFolder);
    // two following lines should be done inseide cropper controller.
    //cropperController.args.rawDataName = rawDataName;
    //cropperController.args.xmlFilename = xmlFilename;

}
