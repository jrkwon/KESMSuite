#include "Setup.h"

Setup::Setup()
{
    debug = true;
    imageFileExtension = ".jpg";
    minPixelIntensity = 0;
    maxPixelIntensity = 255;
    backGroundColor = minPixelIntensity;

    dirNames.cropped = "Cropped";

    QString kesmSuitePathName;
    Executables executables;
    Controllers controllers;

    // image processors info
    TissueAreaDetectorInfo tissueAreaDetectorInfo;
    TissueAreaOutlierRemoverInfo tissueAreaOutlierRemoverInfo;
    RelighterInfo relighterInfo;
    MergerInfo mergerInfo;
    ComposerInfo composerInfo;
}
