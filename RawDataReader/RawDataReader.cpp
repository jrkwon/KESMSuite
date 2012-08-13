#include "RawDataReader.h"

#include <QtCore/QCoreApplication>
#include <QObject>
#include <QStringList>
#include <QFile>
#include <QXmlStreamReader>
#include <QtDebug>
#include <QSharedMemory>
#include <QVector>
#include <QProcess>
#include <iostream>

KESM_NAMESPACE_START

RawDataReader::RawDataReader()
{
    rawData.tissueWidth = -1; // this should be calculated from raw data info.
}

bool RawDataReader::readFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }

    reader.setDevice(&file);
    reader.readNext();
    while(!reader.atEnd()) {
        if(reader.isStartElement()) {
            if(reader.name() == kKESM) {
//                std::cerr << "Found kesm and start to read date" << std::endl;
                rawData.name = reader.attributes().value("name").toString();
                readRawDataElement();
            }
            else {
                reader.raiseError(QObject::tr("Not a KESM data file"));
            }
        }
        else {
            reader.readNext();
        }
    }

//    // see results temporarily
//    qDebug() << "Name:     " << qPrintable(rawData.name);
//    qDebug() << "Year:     " << qPrintable(rawData.date.year);
//    qDebug() << "Month:    " << qPrintable(rawData.date.month);
//    qDebug() << "Day:      " << qPrintable(rawData.date.day);
//    qDebug() << "Animal:   " << qPrintable(rawData.specimen.animal);
//    qDebug() << "Organ:    " << qPrintable(rawData.specimen.organ);
//    qDebug() << "Descr.:   " << qPrintable(rawData.specimen.description);
//    qDebug() << "Width:    " << rawData.pixel.width;
//    qDebug() << "Height:   " << rawData.pixel.height;
//    qDebug() << "Edge:     " << qPrintable(rawData.cutting.edge);
//    qDebug() << "Columns:  " << rawData.cutting.columns;
//    qDebug() << "Distance: " << rawData.cutting.distance;
//    qDebug() << "Depth:    " << rawData.cutting.depth;

    rawData.tissueWidth = rawData.cutting.distance / rawData.pixel.width;
//    qDebug()  << "Tissue W:  " << rawData.tissueWidth;

    file.close();
    if(reader.hasError()) {
        std::cerr << "Error: Failed to parse file "
                  << qPrintable(fileName) << ": "
                  << qPrintable(reader.errorString()) << std::endl;
        return false;
    }
    else if(file.error() != QFile::NoError) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
    }
    return true;
}

void RawDataReader::readRawDataElement()
{
    reader.readNext();
    while(!reader.atEnd())
    {
        if(reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if(reader.isStartElement()) {
            if(reader.name() == kDATE) {
                readDateElement();
            }
            else if(reader.name() == kSPECIMEN) {
                readSpecimenElement();
            }
            else if(reader.name() == kPIXEL) {
                readPixelElement();
            }
            else if(reader.name() == kCUTTING) {
                readCuttingElement();
            }
            else if(reader.name() == kTissue) {
                readTissueElement();
            }
            else {
                skipUnknownElement();
            }
        }
        else {
            reader.readNext();
        }
    }
}

// -----------------------------------------------------------------
// date element
void RawDataReader::readDateElement()
{
    reader.readNext();
    while(!reader.atEnd()) {
        if(reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if(reader.isStartElement()) {
            if(reader.name() == kYEAR) {
                readYearElement();
            }
            else if(reader.name() == kMONTH) {
                readMonthElement();
            }
            else if(reader.name() == kDAY) {
                readDayElement();
            }
            else {
                skipUnknownElement();
            }
        }
        else {
            reader.readNext();
        }
    }
}

void RawDataReader::readYearElement()
{
    rawData.date.year = reader.readElementText();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

void RawDataReader::readMonthElement()
{
    rawData.date.month = reader.readElementText();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

void RawDataReader::readDayElement()
{
    rawData.date.day = reader.readElementText();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

// -----------------------------------------------------------------
// specimen element
void RawDataReader::readSpecimenElement()
{
    reader.readNext();
    while(!reader.atEnd()) {
        if(reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if(reader.isStartElement()) {
            if(reader.name() == kANIMAL) {
                readAnimalElement();
            }
            else if(reader.name() == kORGAN) {
                readOrganElement();
            }
            else if(reader.name() == kDESCRIPTION) {
                readDescriptionElement();
            }
            else {
                skipUnknownElement();
            }
        }
        else {
            reader.readNext();
        }
    }
}

void RawDataReader::readAnimalElement()
{
    rawData.specimen.animal = reader.readElementText();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

void RawDataReader::readOrganElement()
{
    rawData.specimen.organ = reader.readElementText();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

void RawDataReader::readDescriptionElement()
{
    rawData.specimen.description = reader.readElementText();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

// -----------------------------------------------------------------
// pixel element
void RawDataReader::readPixelElement()
{
    reader.readNext();
    while(!reader.atEnd()) {
        if(reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if(reader.isStartElement()) {
            if(reader.name() == kWIDTH) {
                readWidthElement();
            }
            else if(reader.name() == kHEIGHT) {
                readHeightElement();
            }
            else {
                skipUnknownElement();
            }
        }
        else {
            reader.readNext();
        }
    }
}

void RawDataReader::readWidthElement()
{
    rawData.pixel.width = reader.readElementText().toFloat();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

void RawDataReader::readHeightElement()
{
    rawData.pixel.height = reader.readElementText().toFloat();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

// -----------------------------------------------------------------
// cutting element
void RawDataReader::readCuttingElement()
{
    reader.readNext();
    while(!reader.atEnd()) {
        if(reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if(reader.isStartElement()) {
            if(reader.name() == kEDGE) {
                readEdgeElement();
            }
            else if(reader.name() == kCOLUMNS) {
                readColumnsElement();
            }
            else if(reader.name() == kDISTANCE) {
                readDistanceElement();
            }
            else if(reader.name() == kDEPTH) {
                readDepthElement();
            }
            else if(reader.name() == kMinZ) {
                readMinZElement();
            }
            else if(reader.name() == kMaxZ) {
                readMaxZElement();
            }
            else {
                skipUnknownElement();
            }
        }
        else {
            reader.readNext();
        }
    }
}

void RawDataReader::readEdgeElement()
{
    if(reader.readElementText().compare(kLeft, Qt::CaseInsensitive) == 0)
        rawData.cutting.edge = KESLeft;
    else
        rawData.cutting.edge = KESRight;

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

void RawDataReader::readColumnsElement()
{
    rawData.cutting.columns = reader.readElementText().toInt();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

void RawDataReader::readDistanceElement()
{
    rawData.cutting.distance = reader.readElementText().toInt();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

void RawDataReader::readDepthElement()
{
    rawData.cutting.depth = reader.readElementText().toFloat();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

void RawDataReader::readMinZElement()
{
    rawData.cutting.minz = reader.readElementText().toFloat();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

void RawDataReader::readMaxZElement()
{
    rawData.cutting.maxz = reader.readElementText().toFloat();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}


// -----------------------------------------------------------------
// tissue element
void RawDataReader::readTissueElement()
{
    reader.readNext();
    while(!reader.atEnd()) {
        if(reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if(reader.isStartElement()) {
            if(reader.name() == kWIDTH) {
                readTissueWidthElement();
            }
            else if(reader.name() == kHEIGHT) {
                readTissueHeightElement();
            }
            else {
                skipUnknownElement();
            }
        }
        else {
            reader.readNext();
        }
    }
}

void RawDataReader::readTissueWidthElement()
{
    rawData.tissue.width = reader.readElementText().toInt();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

void RawDataReader::readTissueHeightElement()
{
    rawData.tissue.height = reader.readElementText().toInt();

    if(reader.isEndElement()) {
        reader.readNext();
    }
}

// -----------------------
// skip unknown
void RawDataReader::skipUnknownElement()
{
    reader.readNext();
    while(!reader.atEnd()) {
        if(reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if(reader.isStartElement()) {
            skipUnknownElement();
        }
        else {
            reader.readNext();
        }
    }
}

KESM_NAMESPACE_END
