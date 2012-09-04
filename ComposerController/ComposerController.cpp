#include <QDebug>
#include <iostream>
#include "ComposerController.h"

KESM_NAMESPACE_START

ComposerController::ComposerController()
{
    isProcessError = isFinished = false;

    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(updateErrorOutput()));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutput()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

}

void ComposerController::updateErrorOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cerr << qPrintable(QString::fromLocal8Bit(errorArray));
}

void ComposerController::updateOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cout << qPrintable(QString::fromLocal8Bit(errorArray));
}

void ComposerController::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(exitStatus == QProcess::CrashExit)
    {
        qDebug() << "Program crashed.";
        isProcessError = true;
    }
    else {
        switch(exitCode) {
        case 0: // success
            qDebug() << "Success.";
            break;
        default:
            qDebug() << "Error.";
            //?? = exitCode;
            break;
        }
    }
    isFinished = true;
}

void ComposerController::processError(QProcess::ProcessError error)
{
    if(error == QProcess::FailedToStart)
    {
        qDebug() << "Program not found";
    }
    isProcessError = isFinished = true;
}

KESM_NAMESPACE_END
