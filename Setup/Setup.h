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

#define kKESMDebug                  1

#define kKESMSuitePathEnv           "KESMSUITE_HOMEPATH"

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
#define kKESMSuiteVersion                                   "1.0"
#define kTissueAreaDetectorVersion                          "1.0"
#define kTissueAreaDetectorControllerVersion                "1.0"
#define kTissueAreaOutlierRemoverVersion                    "1.0"
#define kTissueAreaOutlierRemoverControllerVersion          "1.0"
#define kCropperVersion                                     "1.0"
#define kCropperControllerVersion                           "1.0"
#define kRelighterVersion                                   "1.0"
#define kRelighterControllerVersion                         "1.0"
#define kMergerVersion                                      "1.0"
#define kMergerControllerVersion                            "1.0"
#define kComposerVersion                                    "1.0"
#define kComposerControllerVersion                          "1.0"

//-----------------------------------------------------------------------------
// KESMSuite execution files
#define kTissueAreaDetectorName             "TissueAreaDetector"
#define kCropperName                        "Cropper"
#define kRelighterName                      "Relighter"
#define kMergerName                         "Merger"
#define kComposerName                       "Composer"

#define kProcessExt                         ".exe"

#ifdef kKESMDebug

#define kDebugPartialPath                   "-build-desktop-Qt_4_7_4_for_Desktop_-_MSVC2008__Qt_SDK__Debug/debug"
#define kTissueAreaDetectorProcessName      kTissueAreaDetectorName""kDebugPartialPath"/"kTissueAreaDetectorName""kProcessExt
#define kCropperProcessName                 kCropperName""kDebugPartialPath"/"kCropperName""kProcessExt
#define kRelighterProcessName               kRelighterName""kDebugPartialPath"/"kRelighterName""kProcessExt
#define kMergerProcessName                  kMergerName""kDebugPartialPath"/"kMergerName""kProcessExt
#define kComposerProcessName                kComposerName""kDebugPartialPath"/"kComposerName""kProcessExt

#else

#define kReleasePartialPath                 "bin"
#define kTissueAreaDetectorProcessName      kReleasePartialPath"/"kTissueAreaDetectorName""kProcessExt
#define kCropperProcessName                 kReleasePartialPath"/"kCropperName""kProcessExt
#define kRelighterProcessName               kReleasePartialPath"/"kRelighterName""kProcessExt
#define kMergerProcessName                  kReleasePartialPath"/"kMergerName""kProcessExt
#define kComposerProcessName                kReleasePartialPath"/"kComposerName""kProcessExt


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


#endif // SETUP_H
