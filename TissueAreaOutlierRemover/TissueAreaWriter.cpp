#include "TissueAreaWriter.h"

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

TissueAreaWriter::TissueAreaWriter(KesmData::Column &column) {
    this->column = column;

//    this->column.id = column.id;
//    this->column.width = column.width;

//    RawImage images[2];
//    images[0].name = "20080423_121806_x148.0967y26.6286z10.6770_t0.001000_v20.9807";
//    images[0].startX = 937;
//    images[0].valid = true;

//    images[1].name = "20080423_121806_x148.0967y26.6286z10.6770_t0.001000_v20.9807";
//    images[1].startX = -1;
//    images[1].valid = false;

//    column.images.append(images[0]);
//    column.images.append(images[1]);
}

bool TissueAreaWriter::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        std::cerr << "Error: Cannot write file "
                  << qPrintable(fileName) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return false;
    }

    writer.setDevice(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    writer.writeStartElement(kCOLUMN);
    writer.writeAttribute(kID, QString::number(column.id));
    writer.writeAttribute(kWIDTH, QString::number(column.width));

    for (int i = 0; i < column.images.size(); i++) {
        writer.writeStartElement(kIMAGE);
        writer.writeAttribute(kNAME, column.images[i].name);

        writer.writeTextElement(kSTARTX, QString::number(column.images[i].startX));
//        writer.writeTextElement(kSTARTX_ORG, QString::number(column.images[i].orgStartX));
//        writer.writeTextElement(kADJUST, column.images[i].adjust ? kTRUE : kFALSE);
        writer.writeTextElement(kVALID, column.images[i].valid ? kTRUE : kFALSE);
        writer.writeTextElement(kCHANGED, column.images[i].changed ? kTRUE : kFALSE);

        writer.writeEndElement();
    }

    writer.writeEndDocument();
    return true;
}

bool TissueAreaWriter::writeCSVFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        std::cerr << "Error: Cannot write file "
                  << qPrintable(fileName) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return false;
    }

    QTextStream out(&file);
    out << kNAME << ", " << kSTARTX << "\n";

    for (int i = 0; i < column.images.size(); i++) {
        out << column.images[i].name << ", " << column.images[i].startX << "\n";
    }

    file.close();
    return true;
}

KESM_NAMESPACE_END
