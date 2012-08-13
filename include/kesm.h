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
#define kBackgroundColor            0xFF

#define kCROPPED_FOLDER_NAME        "Cropped"
#define kRELIGHTED_FOLDER_NAME      "Relighted"
#define kMERGED_FOLDER_NAME         "Merged"
#define kMergerInfoFolderName       "MergerInfo"
#define kTILED_FOLDER_NAME          "Tiled"
#define kSCALED_FOLDER_NAME         "Scaled"

//-----------------------------------------------------------------------------
// KESM Suite version
#define kKESMSuiteVersion                                   "0.8"
#define kTissueAreaDetectorVersion                          kKESMSuiteVersion
#define kTissueAreaDetectorControllerVersion                kKESMSuiteVersion
#define kTissueAreaOutlierRemoverVersion                    kKESMSuiteVersion
#define kTissueAreaOutlierRemoverControllerVersion          kKESMSuiteVersion
#define kCropperVersion                                     kKESMSuiteVersion
#define kCropperControllerVersion                           kKESMSuiteVersion
#define kRelighterVersion                                   kKESMSuiteVersion
#define kRelighterControllerVersion                         kKESMSuiteVersion
#define kMergerVersion                                      kKESMSuiteVersion
#define kMergerControllerVersion                            kKESMSuiteVersion

//-----------------------------------------------------------------------------
// Tissue Area Detector

#define KESM_SUITE_PATH                     "K:/Projects/KESMSuite/"
#ifdef kKESM_DEBUG
#define kTISSUE_AREA_DETECTOR               KESM_SUITE_PATH"TissueAreaDetector-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/TissueAreaDetector.exe"
#define kTISSUE_AREA_DETECTOR_CONTROLLER    KESM_SUITE_PATH"TissueAreaDetectorController-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/TissueAreaDetectorController.exe"
#define kTISSUE_AREA_OUTLIER_REMOVER        KESM_SUITE_PATH"TissueAreaOutlierRemover-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/TissueAreaOutlierRemover.exe"
#define kCROPPER                            KESM_SUITE_PATH"Cropper-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/Cropper.exe"
#define kRELIGHTER                          KESM_SUITE_PATH"Relighter-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/Relighter.exe"
#define kMergerProcessName                  KESM_SUITE_PATH"Merger-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/Merger.exe"
#else
#define kTISSUE_AREA_DETECTOR               KESM_SUITE_PATH"bin/TissueAreaDetector.exe"
#define kTISSUE_AREA_DETECTOR_CONTROLLER    KESM_SUITE_PATH"bin/TissueAreaDetectorController.exe"
#define kTISSUE_AREA_OUTLIER_REMOVER        KESM_SUITE_PATH"bin/TissueAreaOutlierRemover.exe"
#define kCROPPER                            KESM_SUITE_PATH"bin/Cropper.exe"
#define kRELIGHTER                          KESM_SUITE_PATH"bin/Relighter.exe"
#define kMergerProcessName                  KESM_SUITE_PATH"bin/Merger.exe"
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

enum KnifeEdgeSide {
    KESLeft, KESRight
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

//-----------------------------------------------------------------------------
// Merger & GenMergerInfo
#define kMergerInfoFolder                       "MergerInfo"
#define kMergerInfoFile                         "MergerInfo.txt"
#define kMergerInfoFormat                       "i%05dz%.4f.txt"
#define kInvertImageFlag                             "i:"

#define kTagNumberOfImagesToMerge   "NumberOfImagesToMerge"
#define kTagImageWidth              "ImageWidth"
#define kTagImageHeight             "ImageHeight"
#define kTagBaseDir                 "BaseDir"
#define kTagEdge                    "Edge"
#define kLeft                       "Left"
#define kRight                      "Right"

KESM_NAMESPACE_END

#endif // KESM_H
