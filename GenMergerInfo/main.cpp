#include <QtCore/QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <iostream>
#include "GenMergerInfo.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    std::cout << ">>> KESM GenMergerInfo ver " << kMergerVersion << "<<<" << std::endl;
    std::cout << "Generate information files for Merger." << std::endl;

    if(args.size() < 3)
    {
        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName) << " <Input Folder> <Output Folder>" << std::endl;
        std::cout << "Input Folder: Where column folders are located." << std::endl;
        std::cout << "Output Folder: Merged folder." << std::endl;
        return kKESM_ERROR;
    }

    QString inputFolderName(args[1]);
    QString outputFolderName(args[2]);
    std::cout << "outputFolderName:      " << qPrintable(inputFolderName) << std::endl;
    std::cout << "outputFolderName: " << qPrintable(outputFolderName) << std::endl;

    //------------------------------------------------------------------------
    // Merger
    KESM::GenMergerInfo genMergerInfo(inputFolderName, outputFolderName);
    genMergerInfo.execute();

    return kKESM_OK; //a.exec();
}
