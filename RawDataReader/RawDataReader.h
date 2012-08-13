#ifndef RAWDATAREADER_H
#define RAWDATAREADER_H

#include "../include/kesm.h"
#include <QString>
#include <QXmlStreamReader>

KESM_NAMESPACE_START

// ---------------------------
// string constants
#define kVERSION        "1.0"
#define kKESM           "kesm"
#define kNAME           "name"

#define kDATE           "date"
#define kYEAR           "year"
#define kMONTH          "month"
#define kDAY            "day"

#define kSPECIMEN       "specimen"
#define kANIMAL         "animal"
#define kORGAN          "organ"
#define kDESCRIPTION    "description"

#define kPIXEL          "pixel"
#define kWIDTH          "width"
#define kHEIGHT         "height"

#define kCUTTING        "cutting"
#define kEDGE           "edge"
#define kCOLUMNS        "columns"
#define kDISTANCE       "distance"
#define kDEPTH          "depth"
#define kMinZ           "minz"
#define kMaxZ           "maxz"

#define kTissue         "tissue"
//#define kWIDTH          "width"
//#define kHEIGHT         "height"


class RawData {
    struct Date {
        QString year, month, day;
    };

    struct Specimen {
        QString animal, organ, description;
    };

    struct Pixel {
        float width;
        float height;
    };

    struct Cutting {
        KnifeEdgeSide edge;
        int columns;
        int distance;
        float depth;
        float minz;
        float maxz;
    };

    struct Tissue {
        int width;
        int height;
    };

public:
    QString name;
    struct Date date;
    struct Specimen specimen;
    struct Pixel pixel;
    struct Cutting cutting;
    struct Tissue tissue;
    ////////////////////////////
    // actual tissue area width
    // tissueWidth = cutting.distance/pixel.width
    int tissueWidth;
};

class RawDataReader
{
public:
    RawDataReader();
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
    void readMinZElement();
    void readMaxZElement();
    // -- Tissue
    void readTissueElement();
    void readTissueWidthElement();
    void readTissueHeightElement();

    void skipUnknownElement();

    QXmlStreamReader reader;
};

KESM_NAMESPACE_END

#endif // RAWDATAREADER_H
