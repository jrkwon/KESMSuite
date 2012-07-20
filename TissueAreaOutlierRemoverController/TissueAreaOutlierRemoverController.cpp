#include <QDebug>
#include <iostream>
#include "TissueAreaOutlierRemoverController.h"

KESM_NAMESPACE_START

TissueAreaOutlierRemoverController::TissueAreaOutlierRemoverController(QObject *parent) :
    QObject(parent)
{
    isProcessError = isFinished = false;

    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(updateErrorOutput()));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutput()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
}

void TissueAreaOutlierRemoverController::updateErrorOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cerr << qPrintable(QString::fromLocal8Bit(errorArray));
}

void TissueAreaOutlierRemoverController::updateOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cout << qPrintable(QString::fromLocal8Bit(errorArray));
}

void TissueAreaOutlierRemoverController::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(exitStatus == QProcess::CrashExit)
    {
        qDebug() << "Program crashed.";
        isProcessError = true;
    }
    else {
        switch(exitCode) {
        case 0: // success code for cropper
            qDebug() << "Succeeded.";
            break;
        default:
            qDebug() << "Error occurred.";
            //?? = exitCode;
            break;
        }
    }
    isFinished = true;
}

void TissueAreaOutlierRemoverController::processError(QProcess::ProcessError error)
{
    if(error == QProcess::FailedToStart)
    {
        qDebug() << "Cropper program not found";
    }
    isProcessError = isFinished = true;
}

KESM_NAMESPACE_END
