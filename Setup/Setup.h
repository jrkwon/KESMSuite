#ifndef SETUP_H
#define SETUP_H

#include <QString>
#include <QVector>

#define KESM_NAMESPACE_START namespace KESM {
#define KESM_NAMESPACE_END   }

///////////////////////////////////////////////////////////////
// system init values
#define kKESM_DEBUG                 1

#define kSourceImageFileExtension   ".jpg"
#define kBackgroundColor            0xFF

#define kCroppedFolderName          "Cropped"
#define kRelightedFolderName        "Relighted"
#define kMergedFolderName           "Merged"
#define kMergerInfoFolderName       "MergerInfo"

#define kTILED_FOLDER_NAME          "Tiled"
#define kSCALED_FOLDER_NAME         "Scaled"
#define kRotatedFolderName          "Rotated"
#define kAttenuatedFolderName       "Attenuated."  // is followed by the number of images

#define kMaxProcessWaitTime         1800000  // 30 min = 30*60 second = 1,800,000 millisec

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
#define kComposerVersion                                    kKESMSuiteVersion
#define kComposerControllerVersion                          kKESMSuiteVersion

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
#define kComposerProcessName                KESM_SUITE_PATH"Composer-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/Composer.exe"
#else
#define kTISSUE_AREA_DETECTOR               KESM_SUITE_PATH"bin/TissueAreaDetector.exe"
#define kTISSUE_AREA_DETECTOR_CONTROLLER    KESM_SUITE_PATH"bin/TissueAreaDetectorController.exe"
#define kTISSUE_AREA_OUTLIER_REMOVER        KESM_SUITE_PATH"bin/TissueAreaOutlierRemover.exe"
#define kCROPPER                            KESM_SUITE_PATH"bin/Cropper.exe"
#define kRELIGHTER                          KESM_SUITE_PATH"bin/Relighter.exe"
#define kMergerProcessName                  KESM_SUITE_PATH"bin/Merger.exe"
#define kComposerProcessName                KESM_SUITE_PATH"bin/Composer.exe"
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


//-----------------------------------------------------------------------------
// Composer
// Base file name format <= indexed image file format
// iNNNNNNzZ.ZZZZ.jpg
// NNNNNN: image index
// Z.ZZZZ: z axis position in the KESM
//

const QString indexedImagefileNameFormat = "NNNNN";
#define kIndexedImageFileNameFormat     "iNNNNN" // zZ.ZZZZ.jpg"
#define kZPosFileNameFormat             "Z.ZZZZ"

#define kComposerUnitDepth              5



class Setup
{
public:
    Setup();

public:
    // enums
    enum ErrorCode { Error = -1, Ok = 1 };
    enum KnifeEdgeSide { LeftEdge, RightEdge };

    // structs
    struct tagNames {
        QString column;
        QString idString;
        QString width;
        QString image;
        QString name;
        QString startX;
        QString newStartX;
        QString valide;
        QString adjust;
        QString changed;
        QString trueString;
        QString falseString;
    };

    struct DirNames {
        QString cropped;
        QString relighted;
        QString merged;
        QString mergerInfo;
        QString rotated;
        QString attenuated;
        // QString scaled;
        // QStrign tiled; ??
    };

    struct Executable {
        QString version;
        QString filePathName;
    };

    struct Executables {
        Executable tissueAreaDetector;
        Executable tissueAreaOutlierRemover;
        Executable cropper;
        Executable relighter;
        Executable merger;
        Executable composer;
    };

    struct Controllers {
        Executable tissueAreaDetectorController;
        Executable tissueAreaOutlierRemoverController;
        Executable cropperController;
        Executable relighterController;
        Executable mergerController;
        Executable composerController;
    };

    // paramters for processors

    struct TissueAreaDetectorInfo {
        QString rawDataInfoFileName;
        QString templateFileName;
        QString numberOfDigitsInColumn;
        QString rightEdgeWindowSize;
    };

    struct TissueAreaOutlierRemoverInfo {
        int imageChunkThreshold;
        int maxRightEdgeDifference;
        int maxConsecutiveWrongImages;
        int defaultFactorForWindowSize;
        int maxOutlerImages;
    };

    struct RelighterInfo {
        int minIntensityForRelight;
        int maxIntensityForRelgith;
    };

    struct MergerInfo {
        QString dirName;
        QString fileName;
        QString fileNameFormat;
        QString invertImageFlag;

        QString tagNumberOfImagesToMerge;
        QString tagImageWidth;
        QString tagImageHeight;
        QString tagBaseDir;
        QString tagEdge;
        QString tagLeft;
        QString tagRight;

    };

    struct ComposerInfo {
        QString indexedImageFileNameFormat;
        QString zPosFileNameFormat;
        QString unitDepth;
    };

public:
    // environments
    bool debug;
    ErrorCode errorCode;
    QString imageFileExtension;
    char minPixelIntensity;
    char maxPixelIntensity;
    char backGroundColor;

    DirNames dirNames;

    QString kesmSuitePathName;
    Executables executables;
    Controllers controllers;

    // image processors info
    TissueAreaDetectorInfo tissueAreaDetectorInfo;
    TissueAreaOutlierRemoverInfo tissueAreaOutlierRemoverInfo;
    RelighterInfo relighterInfo;
    MergerInfo mergerInfo;
    ComposerInfo composerInfo;
};

KESM_NAMESPACE_END

#endif // SETUP_H
