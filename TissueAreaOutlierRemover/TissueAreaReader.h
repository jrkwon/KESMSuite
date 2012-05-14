#ifndef TISSUEAREAREADER_H
#define TISSUEAREAREADER_H

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

class TissueAreaReader
{
public:
    TissueAreaReader();
    bool readFile(const QString &fileName);

public:
    KesmData::Column column;

private:
    void readColumnElement();
    // -- Image
    void readImageElement(KesmData::RawImage& image);
    int  readStartXElement();
    bool readValidElement();
    bool readChangedElement();

    void skipUnknownElement();

    QXmlStreamReader reader;
};

KESM_NAMESPACE_END

#endif // TISSUEAREAREADER_H
