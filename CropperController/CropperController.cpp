#include <QDebug>
#include "CropperController.h"
#include <iostream>

KESM_NAMESPACE_START

CropperController::CropperController()
{
    isProcessError = isFinished = false;

    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(updateErrorOutput()));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutput()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

}

void CropperController::updateErrorOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cerr << qPrintable(QString::fromLocal8Bit(errorArray));
}

void CropperController::updateOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cout << qPrintable(QString::fromLocal8Bit(errorArray));
}

void CropperController::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(exitStatus == QProcess::CrashExit)
    {
        qDebug() << "Cropper program crashed.";
        isProcessError = true;
    }
    else {
        switch(exitCode) {
        case -1:
            qDebug() << "Crop failed.";
            break;
        case 0: // success code for cropper
            qDebug() << "Crop succeeded.";
            break;
        default:
            //qDebug() << "Error occurred.";
            //?? = exitCode;
            break;
        }
    }
    isFinished = true;
}

void CropperController::processError(QProcess::ProcessError error)
{
    if(error == QProcess::FailedToStart)
    {
        qDebug() << "Cropper program not found";
    }
    isProcessError = isFinished = true;
}

KESM_NAMESPACE_END
