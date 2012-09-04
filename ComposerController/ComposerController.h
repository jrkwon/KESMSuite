#ifndef COMPOSERCONTROLLER_H
#define COMPOSERCONTROLLER_H

#include "../include/kesm.h"
#include <QObject>
#include <QProcess>

// moc compiler does not recognize this definition.
//KESM_NAMESPACE_START
namespace KESM {

class ComposerController : public QObject
{
    Q_OBJECT

public:
    ComposerController();

public slots:
    void updateOutput();
    void updateErrorOutput();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processError(QProcess::ProcessError error);

public:
    QProcess process;
    bool isFinished;
    bool isProcessError;
};

}
//KESM_NAMESPACE_END


#endif // COMPOSERCONTROLLER_H
