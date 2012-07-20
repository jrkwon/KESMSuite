#ifndef RELIGHTERCONTROLLER_H
#define RELIGHTERCONTROLLER_H

#include "../include/kesm.h"
#include <QObject>
#include <QProcess>

// moc compiler does not recognize this definition.
//KESM_NAMESPACE_START
namespace KESM {

class RelighterController : public QObject
{
    Q_OBJECT

public:
    RelighterController();

    struct Args {
        QString sourceDataFolder;
        QString destinationDataFolder;
        //QString columnNumber;
        QString rawDataName;  // the data name
        //QString templateFilename;
        //QString xmlFilename;
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

    Args args;
};

}
//KESM_NAMESPACE_END

#endif // RELIGHTERCONTROLLER_H
