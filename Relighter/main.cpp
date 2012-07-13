#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <iostream>
#include "Relighter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    std::cout << ">>> KESM Relighter ver " << kRelighterVersion << " <<<" << std::endl;
    std::cout << "Normalize the image intensity level of an image." << std::endl;

    if(args.size() != 3)
    {
        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " SourceImageFile RelightedImageFile" << std::endl;
        return kKESM_ERROR;
    }

    QString sourceImageFileName(args[1]);
    QString relightedImageFileName(args[2]);

    std::cout << "SrcImage: " << qPrintable(sourceImageFileName) << std::endl;
    std::cout << "Relighted: " << qPrintable(relightedImageFileName) << std::endl;

    //------------------------------------------------------------------------
    // Relighter
    KESM::Relighter relighter;
    relighter.execute(sourceImageFileName, relightedImageFileName);

    return kKESM_OK; //a.exec();
}
