#include "TissueAreaDetectorController.h"
#include <QDebug>
#include <iostream>

KESM_NAMESPACE_START

TissueAreaDetectorController::TissueAreaDetectorController()
{
    isProcessError = isFinished = false;
    startX = -1;

    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(updateErrorOutput()));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutput()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

}

void TissueAreaDetectorController::updateErrorOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cerr << qPrintable(QString::fromLocal8Bit(errorArray));
}

void TissueAreaDetectorController::updateOutput()
{
    QByteArray errorArray = process.readAllStandardError();
    std::cout << qPrintable(QString::fromLocal8Bit(errorArray));
}

void TissueAreaDetectorController::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(exitStatus == QProcess::CrashExit)
    {
        qDebug() << "Tisse Area Detector program crashed.";
        isProcessError = true;
    }
    else {
        switch(exitCode) {
        case -1:
            qDebug() << "Detection failed.";
            break;
        case 0:
            qDebug() << "Error occurred.";
            break;
        default:
            //qDebug() << "Detection succeeded.";
            startX = exitCode;
        }
    }
    isFinished = true;
}

void TissueAreaDetectorController::processError(QProcess::ProcessError error)
{
    if(error == QProcess::FailedToStart)
    {
        qDebug() << "Conversion program not found";
    }
    isProcessError = isFinished = true;
}

KESM_NAMESPACE_END
