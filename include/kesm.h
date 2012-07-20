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
#define kTissueAreaDetectorVersion                          "0.8"
#define kTissueAreaDetectorControllerVersion                "0.8"
#define kTissueAreaOutlierRemoverVersion                    "0.8"
#define kTissueAreaOutlierRemoverControllerVersion          "0.8"
#define kCropperVersion                                     "0.8"
#define kCropperControllerVersion                           "0.8"
#define kRelighterVersion                                   "0.8"
#define kRelighterControllerVersion                         "0.8"

//-----------------------------------------------------------------------------
// Tissue Area Detector

#define KESM_SUITE_PATH                     "K:/Projects/KESMSuite/"
#ifdef kKESM_DEBUG
#define kTISSUE_AREA_DETECTOR               KESM_SUITE_PATH"TissueAreaDetector-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/TissueAreaDetector.exe"
#define kTISSUE_AREA_DETECTOR_CONTROLLER    KESM_SUITE_PATH"TissueAreaDetectorController-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/TissueAreaDetectorController.exe"
#define kTISSUE_AREA_OUTLIER_REMOVER        KESM_SUITE_PATH"TissueAreaOutlierRemover-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/TissueAreaOutlierRemover.exe"
#define kCROPPER                            KESM_SUITE_PATH"Cropper-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/Cropper.exe"
#define kRELIGHTER                          KESM_SUITE_PATH"Relighter-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/Relighter.exe"
#else
#define kTISSUE_AREA_DETECTOR               KESM_SUITE_PATH"bin/TissueAreaDetector.exe"
#define kTISSUE_AREA_DETECTOR_CONTROLLER    KESM_SUITE_PATH"bin/TissueAreaDetectorController.exe"
#define kTISSUE_AREA_OUTLIER_REMOVER        KESM_SUITE_PATH"bin/TissueAreaOutlierRemover.exe"
#define kCROPPER                            KESM_SUITE_PATH"bin/Cropper.exe"
#define kRELIGHTER                          KESM_SUITE_PATH"bin/Relighter.exe"
#endif


//-----------------------------------------------------------------------------
// Tissue Area Detect Controller

#define kRAW_DATA_INFO_FILENAME     "RawDataInfo.xml"
#define kTEMPLATE_FILENAME          "Template"

#define kCOLUMN_DIGITS              "5"     // the number of digits in a column name

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
#define kNEW_STARTX      "newStartx"
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
        int startX;     // tissue start point from auto detection
        bool valid;     // not used for now
        bool changed;   // adjusted by outlier remover
        //int adjStartX;  // after removing outlers
        int newStartX;  // start x after smoothing
    };

    struct Column {
        int id;         // column number
        int width;      // tissue area width

        QVector<RawImage> images;
    };

};

#define kMinPixelIntensity                      0
#define kMaxPixelIntensity                      255

//-----------------------------------------------------------------------------
// Tissue Area Outlier Remover
#define kImageChunkThreshold                    10
#define kMaxRightEdgeDifference                 15
#define kMaxConsecutiveWrongImages              10
#define kDefaultFactorForWindowSize             2
#define kMaxOutlierImages                       5 // 10 //5

//-----------------------------------------------------------------------------
// Relighter
#define kMinIntensity4Relight                   0
#define kMaxIntensity4Relight                   140

KESM_NAMESPACE_END

#endif // KESM_H
