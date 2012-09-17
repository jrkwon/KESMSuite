#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <iostream>
#include "Composer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    std::cout << ">>> KESM Composer ver " << kComposerVersion << " <<< " << std::endl;
    std::cout << "Make intensity attenuated images." << std::endl;

    if(args.size() < 4)
    {
        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " baseFilePathName outputPathName numberOfImages" << std::endl;
        return kKESM_ERROR;
    }

    QString baseFilePathName(args[1]);
    QString outputPathName(args[2]);
    int numberOfImages = args[3].toInt();

    QString logTag;
    if(args.size() >= 5)
        logTag = args[4];

    std::cout << "Base FilePath:    " << qPrintable(baseFilePathName) << std::endl;
    std::cout << "Output Path:      " << qPrintable(outputPathName) << std::endl;
    std::cout << "Number of images: " << numberOfImages << std::endl;

    //------------------------------------------------------------------------
    // Composer
    KESM::Composer composer(baseFilePathName, outputPathName, numberOfImages);

    // for debug or data analysis
    composer.logProcessingTime = true; // let's make log files
    composer.logTag = logTag;
    composer.execute(false); // let's not overwrite

    return kKESM_OK; //a.exec();
}
