#ifndef SETUP_H
#define SETUP_H

#include <QString>
#include <QVector>

#define KESM_NAMESPACE_START namespace KESM {
#define KESM_NAMESPACE_END   }

///////////////////////////////////////////////////////////////
// system init values
// ------------------
// these defintions are used to initialize the Setup object.

#define kKesmDebug                  1

#define kSourceImageFileExtension   ".jpg"
#define kBackgroundColor            0xFF

#define kCroppedDirName             "Cropped"
#define kRelightedDirName           "Relighted"
#define kMergedDirName              "Merged"
#define kMergerInfoDirName          "MergerInfo"

#define kTiledDirName               "Tiled"
#define kScaledDirName              "Scaled"
#define kRotatedDirName             "Rotated"
#define kAttenuatedDirName          "Attenuated."  // is followed by the number of images

#define kMaxProcessWaitTime         1800000  // 30 min = 30*60 second = 1,800,000 millisec

//-----------------------------------------------------------------------------
// KESM Suite version
#define kKESMSuiteVersion                                   "0.8"
#define kTissueAreaDetectorVersion                          "0.8"
#define kTissueAreaDetectorControllerVersion                "0.8"
#define kTissueAreaOutlierRemoverVersion                    "0.8"
#define kTissueAreaOutlierRemoverControllerVersion          "0.8"
#define kCropperVersion                                     "0.8"
#define kCropperControllerVersion                           "0.8"
#define kRelighterVersion                                   "0.8"
#define kRelighterControllerVersion                         "0.8"
#define kMergerVersion                                      "0.8"
#define kMergerControllerVersion                            "0.8"
#define kComposerVersion                                    "0.8"
#define kComposerControllerVersion                          "0.8"

//-----------------------------------------------------------------------------
// KESMSuite execution files

#ifdef kKesmDebug
#define kKESMSuitePath                      "P:/KESMSuite"
#define kTissueAreaDetecor                  kKESMSuitePath"/TissueAreaDetector-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/TissueAreaDetector.exe"
#define kTissueAreaOutlierRemover           kKESMSuitePath"/TissueAreaOutlierRemover-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/TissueAreaOutlierRemover.exe"
#define kCropper                            kKESMSuitePath"/Cropper-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/Cropper.exe"
#define kRelighter                          kKESMSuitePath"/Relighter-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/Relighter.exe"
#define kMergerProcessName                  kKESMSuitePath"/Merger-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/Merger.exe"
#define kComposerProcessName                kKESMSuitePath"/Composer-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug/Composer.exe"
#else
#define kKESMSuitePath                      "P:/KESMSuite"
#define kTissueAreaDetecor                  kKESMSuitePath"/bin/TissueAreaDetector.exe"
#define kTissueAreaOutlierRemover           kKESMSuitePath"/bin/TissueAreaOutlierRemover.exe"
#define kCropper                            kKESMSuitePath"/bin/Cropper.exe"
#define kRelighter                          kKESMSuitePath"/bin/Relighter.exe"
#define kMergerProcessName                  kKESMSuitePath"/bin/Merger.exe"
#define kComposerProcessName                kKESMSuitePath"/bin/Composer.exe"
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
#define kNEW_STARTX     "newStartx"
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
    char backgroundColor;

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
