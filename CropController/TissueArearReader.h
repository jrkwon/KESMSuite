#ifndef TISSUEAREARREADER_H
#define TISSUEAREARREADER_H

#include "../include/kesm.h"
#include <QString>
#include <QXmlStreamReader>

KESM_NAMESPACE_START

class TissueAreaReader
{
public:
    TissueAreaReader();
    bool readFile(const QString &fileName);

public:
    RawData rawData;

private:
    void readRawDataElement();
    // -- Date
    void readDateElement();
    void readYearElement();
    void readMonthElement();
    void readDayElement();
    // -- Specimen
    void readSpecimenElement();
    void readAnimalElement();
    void readOrganElement();
    void readDescriptionElement();
    // -- Pixel
    void readPixelElement();
    void readWidthElement();
    void readHeightElement();
    // -- Cutting
    void readCuttingElement();
    void readEdgeElement();
    void readColumnsElement();
    void readDistanceElement();
    void readDepthElement();

    void skipUnknownElement();

    QXmlStreamReader reader;
};
};

KESM_NAMESPACE_END

#endif // TISSUEAREARREADER_H
