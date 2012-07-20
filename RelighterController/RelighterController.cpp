#include <QDebug>
#include <iostream>
#include "RelighterController.h"

KESM_NAMESPACE_START

RelighterController::RelighterController()
{
    isProcessError = isFinished = false;

    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(updateErrorOutput()));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutput()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

}

void RelighterController::updateErrorOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cerr << qPrintable(QString::fromLocal8Bit(errorArray));
}

void RelighterController::updateOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cout << qPrintable(QString::fromLocal8Bit(errorArray));
}

void RelighterController::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(exitStatus == QProcess::CrashExit)
    {
        qDebug() << "Relighter program crashed.";
        isProcessError = true;
    }
    else {
        switch(exitCode) {
        case 0: // success code for cropper
            qDebug() << "Relight succeeded.";
            break;
        default:
            qDebug() << "Error occurred.";
            //?? = exitCode;
            break;
        }
    }
    isFinished = true;
}

void RelighterController::processError(QProcess::ProcessError error)
{
    if(error == QProcess::FailedToStart)
    {
        qDebug() << "Cropper program not found";
    }
    isProcessError = isFinished = true;
}

KESM_NAMESPACE_END
