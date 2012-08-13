#include <QDebug>
#include <iostream>
#include "MergerController.h"

KESM_NAMESPACE_START

MergerController::MergerController()
{
    isProcessError = isFinished = false;

    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(updateErrorOutput()));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutput()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

}

void MergerController::updateErrorOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cerr << qPrintable(QString::fromLocal8Bit(errorArray));
}

void MergerController::updateOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cout << qPrintable(QString::fromLocal8Bit(errorArray));
}

void MergerController::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
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

void MergerController::processError(QProcess::ProcessError error)
{
    if(error == QProcess::FailedToStart)
    {
        qDebug() << "Program not found";
    }
    isProcessError = isFinished = true;
}

KESM_NAMESPACE_END
