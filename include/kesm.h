#ifndef KESM_H
#define KESM_H

#include <QString>
#include <QVector>

#define KESM_NAMESPACE_START namespace KESM {
#define KESM_NAMESPACE_END   }

KESM_NAMESPACE_START

#define kKESM_DEBUG                 1

#define kKESM_ERROR                 -1
#define kKESM_OK                    0

#define kSOURCE_IMAGE_FILE_EXT      ".jpg"

#define kCROPPED_FOLDER_NAME        "Cropped"
#define kRELIGHTED_FOLDER_NAME      "Relighted"
#define kMERGED_FOLDER_NAME         "Merged"
#define kTILED_FOLDER_NAME          "Tiled"
#define kSCALED_FOLDER_NAME         "Scaled"

//-----------------------------------------------------------------------------
// KESM Suite version
#define kTissueAreaDetectorVersion              "1.0"
#define kTissueAreaDetectorControllerVersion    "1.0"
#define kTissueAreaOutlierRemoverVersion        "1.0"

//-----------------------------------------------------------------------------
// Tissue Area Detector

#ifdef kKESM_DEBUG
#define kTISSUE_AREA_DETECTOR               "K:/Projects/KESMSuite/TissueAreaDetector-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/TissueAreaDetector.exe"
#define kTISSUE_AREA_DETECTOR_CONTROLLER    "K:/Projects/KESMSuite/TissueAreaDetectorController-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/TissueAreaDetectorController.exe"
#else
#define kTISSUE_AREA_DETECTOR               "K:/Projects/KESMSuite/TissueAreaDetector-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/TissueAreaDetector.exe"
#define kTISSUE_AREA_DETECTOR_CONTROLLER    "K:/Projects/KESMSuite/TissueAreaDetectorController/TissueAreaDetectorController.exe"
#endif


//-----------------------------------------------------------------------------
// Tissue Area Detect Controller

#define kRAW_DATA_INFO_FILENAME     "RawDataInfo.xml"
#define kTEMPLATE_FILENAME          "Template"

#define kCOLUMN_DIGITS              "5"

// parameters for images
#define kRIGHT_EDGE_WINDOW_SIZE     30

//-----------------------------------------------------------------------------
// Tissue Area Reader / Writer

#define kCOLUMN         "column"
#define kID             "id"
#define kWIDTH          "width"
#define kIMAGE          "image"
#define kNAME           "name"
#define kSTARTX         "startx"
#define kVALID          "valid"
#define kADJUST         "adjust"
#define kCHANGED        "changed"
#define kTRUE           "true"
#define kFALSE          "false"


//-----------------------------------------------------------------------------
// Tissue Area Reader / Writer
class KesmData {
public:
    struct RawImage {
        QString name;
        int startX;     // tissue start point
        bool valid;     // not used for now
        bool changed;
    };

    struct Column {
        int id;         // column number
        int width;      // tissue area width

        QVector<RawImage> images;
    };

};

//-----------------------------------------------------------------------------
// Tissue Area Outlier Remover
#define kImageChunkThreshold                    10
#define kMaxRightEdgeDifference                 15
#define kMaxConsecutiveWrongImages              10
#define kDefaultFactorForWindowSize             2
#define kMaxOutlierImages                       5 // 10 //5

KESM_NAMESPACE_END

#endif // KESM_H
