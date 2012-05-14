#include "TissueAreaReader.h"

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

TissueAreaReader::TissueAreaReader()
{
}

bool TissueAreaReader::readFile(const QString &fileName)
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
            if(reader.name() == kCOLUMN) {
//                std::cerr << "Found kesm and start to read date" << std::endl;
                column.id = reader.attributes().value(kID).toString().toInt();
                column.width = reader.attributes().value(kWIDTH).toString().toInt();
                readColumnElement();
            }
            else {
                reader.raiseError(QObject::tr("Not a KESM column XML file"));
            }
        }
        else {
            reader.readNext();
        }
    }

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

void TissueAreaReader::readColumnElement()
{
    reader.readNext();
    while(!reader.atEnd())
    {
        if(reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if(reader.isStartElement()) {
            if(reader.name() == kIMAGE) {
                KesmData::RawImage image;
                image.changed = false; // newly added
                image.name = reader.attributes().value(kNAME).toString();
                readImageElement(image);
                column.images.append(image);
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

void TissueAreaReader::readImageElement(KesmData::RawImage& image)
{
    reader.readNext();
    while(!reader.atEnd())
    {
        if(reader.isEndElement()) {
            reader.readNext();
            break;
        }
        if(reader.isStartElement()) {
            if(reader.name() == kSTARTX) {
                image.startX = readStartXElement();
            }
            else if(reader.name() == kVALID) {
                image.valid = readValidElement();
            }
            else if(reader.name() == kCHANGED) {
                image.changed = readChangedElement();
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
// Start X element
int TissueAreaReader::readStartXElement()
{
    int startX = -1;

    startX = reader.readElementText().toInt();

    if(reader.isEndElement()) {
        reader.readNext();
    }
    return startX;
}

// -----------------------------------------------------------------
// Valid element
bool TissueAreaReader::readValidElement()
{
    bool valid = false;

    if(reader.readElementText() == kTRUE)
        valid = true;
    else
        valid = false;

    if(reader.isEndElement()) {
        reader.readNext();
    }
    return valid;
}

// -----------------------------------------------------------------
// Valid element
bool TissueAreaReader::readChangedElement()
{
    bool changed;

    if(reader.readElementText() == kTRUE)
        changed = true;
    else
        changed = false;

    if(reader.isEndElement()) {
        reader.readNext();
    }
    return changed;
}

// -----------------------
// skip unknown
void TissueAreaReader::skipUnknownElement()
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
