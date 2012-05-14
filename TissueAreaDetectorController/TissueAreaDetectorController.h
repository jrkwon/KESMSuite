#ifndef TISSUEAREADETECTORCONTROLLER_H
#define TISSUEAREADETECTORCONTROLLER_H

#include "../include/kesm.h"
#include <QObject>
#include <QProcess>

// moc compiler does not recognize this definition.
//KESM_NAMESPACE_START
namespace KESM {

class TissueAreaDetectorController  : public QObject {
    Q_OBJECT

public:
    TissueAreaDetectorController();

    struct Args {
        QString sourceDataFolder;
        QString destinationDataFolder;
        QString columnNumber;
        QString rawDataName;
        QString templateFilename;
        QString xmlFilename;
    };

public slots:
    void updateOutput();
    void updateErrorOutput();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processError(QProcess::ProcessError error);

public:
    QProcess process;
    bool isFinished;
    bool isProcessError;
    int startX;
    Args args;
};

}
//KESM_NAMESPACE_END

#endif // TISSUEAREADETECTORCONTROLLER_H
