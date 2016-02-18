#include "Setup.h"
#include <QProcessEnvironment>

Setup::Setup()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString kesmSuiteHomePath = env.value("KESMSUITE_HOMEPATH");
    QStringList list = env.toStringList();
    qDebug("%s", kesmSuiteHomePath);


    debug = true;
    imageFileExtension = ".jpg";
    minPixelIntensity = 0;
    maxPixelIntensity = 255;
    backgroundColor = minPixelIntensity;

    dirNames.cropped =      "Cropped";
    dirNames.relighted =    "Relighted";
    dirNames.

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
