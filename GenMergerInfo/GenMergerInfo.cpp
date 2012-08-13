#include "GenMergerInfo.h"
#include <QDir>
#include <QTextStream>

KESM_NAMESPACE_START

GenMergerInfo::GenMergerInfo(QString inputFolderName, QString outputFolderName)
{
    this->inputFolderName = QDir::fromNativeSeparators(inputFolderName);
    this->outputFolderName = QDir::fromNativeSeparators(outputFolderName);
    if(this->inputFolderName.endsWith("/") == false)
        this->inputFolderName.append("/");
    if(this->outputFolderName.endsWith("/") == false)
        this->outputFolderName.append("/");

    QString strRawDataXMLFileName = QString("%1%2%3").arg(this->inputFolderName).arg("../").arg(kRAW_DATA_INFO_FILENAME);
    this->rawDataReader.readFile(strRawDataXMLFileName);
}

bool GenMergerInfo::collectFileInfo()
{
    // get folders
    QDir dir(inputFolderName);
    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    QFileInfoList list = dir.entryInfoList();
    for(int i = 0; i < list.size(); i++)
    {
        FilesInColumn filesInColumn;

        // -----------------
        // get a folder name
        QFileInfo fileInfo = list.at(i);
        QString columnNumber = fileInfo.fileName(); // folder name
        // set column number
        bool ok;
        filesInColumn.columnNumer = columnNumber.toInt(&ok);
        if(ok == false)
            return false;

        // --------------------------------
        // get a list of iles in the column
        QDir dir(fileInfo.absoluteFilePath());
        QStringList filters;
        filters << QString("*%1").arg(kSOURCE_IMAGE_FILE_EXT);
        dir.setNameFilters(filters);
        // set file list
        filesInColumn.files = dir.entryList(QDir::Files);

        columnFileList.append(filesInColumn);
    }

    mergerInfo.numberOfImagesToMerge = columnFileList.size();
    mergerInfo.baseDir = inputFolderName;
    mergerInfo.imageWidth = rawDataReader.rawData.tissue.width;
    mergerInfo.imageHeight = rawDataReader.rawData.tissue.height;
    mergerInfo.edge = rawDataReader.rawData.cutting.edge;

    return true;
}

void GenMergerInfo::createMergerInfo()
{
    // create MergerInfo
    QString mergerInfoFileName = QString("%1%2").arg(outputFolderName).arg(kMergerInfoFile);
    QFile file(mergerInfoFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << kTagNumberOfImagesToMerge << ":\t" << mergerInfo.numberOfImagesToMerge << "\n";
    out << kTagBaseDir << ":\t" << mergerInfo.baseDir << "\n";
    out << kTagImageWidth << ":\t" << mergerInfo.imageWidth << "\n";
    out << kTagImageHeight << ":\t" << mergerInfo.imageHeight << "\n";
    if(mergerInfo.edge == KESLeft)
        out << kTagEdge << ":\t" << kLeft << "\n";
    else
        out << kTagEdge << ":\t" << kRight << "\n";

    file.close();
}

void GenMergerInfo::execute()
{
    if(collectFileInfo() == false)
        return;
    createMergerInfo();

    // should support sub-micron
    long z = rawDataReader.rawData.cutting.minz * 1E4; // * 10000;
    long maxZ = rawDataReader.rawData.cutting.maxz * 1E4;

    for(int indexZ = 0; z <= maxZ; indexZ++)
    {
        // comparing two floating number is not safe because of floating point precision.
        // conver it to a string and compare.
        QString filter;
        filter.sprintf("z%.4f", z/1E4);

        QString mergerInfo4One;
        mergerInfo4One.sprintf(kMergerInfoFormat, indexZ, z/1E4);
        mergerInfo4One.prepend(outputFolderName);

        QFile file(mergerInfo4One);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        for(int i = 0; i < columnFileList.size(); i++)
        {
            QStringList files = columnFileList[i].files.filter(filter);
            if(files.size() == 1)
                out << i << ":\t" << columnFileList[i].columnNumer << "/" << files[0] << "\n";
            else
                out << i << ":\t\n";
        }

        file.close();

        // z: millimeter
        // depth: umeter
        z += rawDataReader.rawData.cutting.depth * 10;
    }
}

KESM_NAMESPACE_END
