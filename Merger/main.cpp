#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <iostream>
#include "Merger.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    std::cout << ">>> KESM Merger ver " << kMergerVersion << " <<< " << std::endl;
    std::cout << "Merge multiple images into an image." << std::endl;

    if(args.size() < 3)
    {
        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " <Source Info Text Filename> <Output Image Filename> -i:on" << std::endl;
        std::cout << qPrintable(fileName )<< " -i:on/off (optional, off is default) invert images" << std::endl;
        return kKESM_ERROR;
    }

    QString imageInfoName(args[1]);
    QString outputImageName(args[2]);
    if(args.size() == 3)
        args.append(kInvertImageFlag"off");

    QString invertFlag(args[3]);
    bool invertImage = false;
    if(invertFlag.compare(kInvertImageFlag"on", Qt::CaseInsensitive) == 0)
        invertImage = true;

    std::cout << "SrcInfo:      " << qPrintable(imageInfoName) << std::endl;
    std::cout << "Output image: " << qPrintable(outputImageName) << std::endl;
    std::cout << "invert: " << qPrintable(invertFlag) << std::endl;

    //------------------------------------------------------------------------
    // Merger
    KESM::Merger merger(imageInfoName, outputImageName, invertImage);
    merger.execute(false); // let's not overwrite

    return kKESM_OK; //a.exec();
}
