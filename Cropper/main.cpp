#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <iostream>
#include "Cropper.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    std::cout << ">>> KESM Cropper ver <<<" << kCropperVersion << std::endl;
    std::cout << "Cut one one rectangular region of the image." << std::endl;

    if(args.size() < 7)
    {
        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " SourceImageFile x y width height CroppedImageFile" << std::endl;
        return kKESM_ERROR;
    }

    QString imageName(args[1]);
    QPoint start(args[2].toInt(), args[3].toInt());
    QSize size(args[4].toInt(), args[5].toInt());
    QRect rect(start, size);
    QString croppedImageName(args[6]);
    std::cout << "SrcImage: " << qPrintable(imageName) << std::endl;
    std::cout << "x:        " << start.x() << std::endl;
    std::cout << "y:        " << start.y() << std::endl;
    std::cout << "width:    " << size.width() << std::endl;
    std::cout << "height:   " << size.height() << std::endl;
    std::cout << "Cropped:  " << qPrintable(croppedImageName) << std::endl;

    //------------------------------------------------------------------------
    // Cropper
    KESM::Cropper cropper;
    cropper.crop(imageName, rect, croppedImageName);

    return kKESM_OK; //a.exec();
}
