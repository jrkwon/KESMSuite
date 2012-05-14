#include <QtCore/QCoreApplication>
#include <QtGlobal>
#include <QStringList>
#include <QFileInfo>
#include <QDebug>
#include <iostream>

#include "../include/kesm.h"
#include "TissueAreaReader.h"
#include "TissueAreaWriter.h"
#include "TissueAreaOutlierRemover.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if(args.size() < 5)
    {
        std::cout << ">>> KESM Tissue Area Outlier Remover ver <<<" << kTissueAreaOutlierRemoverVersion << std::endl;
        std::cout << "Removes outliers and identifies chunks." << std::endl;

        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " SourceXMLFile DestinationXMLFile DestinationTextFile ChunkList" << std::endl;
        return kKESM_ERROR;
    }

    KESM::TissueAreaReader reader;
    reader.readFile(args[1]); // read XML file

    //------------------------------------------------------------------------
    // Outlier remover
    KESM::TissueAreaOutlierRemover remover(&reader.column);

    //------------------------------------------------------------------------
    // Remover
    remover.sourceXMLFilename = args[1];
    remover.destinationXMLFilename = args[2];
    remover.destinationCSVFilename = args[3];
    remover.chunkListFilename = args[4];

    std::cout << ">>> KESM Tissue Area Outlier Remover ver <<<" << kTissueAreaOutlierRemoverVersion << std::endl;
    std::cout << "Removes outliers and identifies chunks." << std::endl;
    std::cout << "SrcXML: " << qPrintable(remover.sourceXMLFilename) << std::endl;
    std::cout << "DstXML: " << qPrintable(remover.destinationXMLFilename) << std::endl;
    std::cout << "DstTxt: " << qPrintable(remover.destinationCSVFilename) << std::endl;
    std::cout << "CnkLst: " << qPrintable(remover.chunkListFilename) << std::endl;

    //------------------------------------------------------------------------
    // Remove outliers and identify chunks
    remover.removeShortBurst();
    /*remover.removeLongBurst(kDefaultFactorForWindowSize);
    remover.removeLongBurst(kDefaultFactorForWindowSize+1);
    remover.removeLongBurst(kDefaultFactorForWindowSize+2);
    remover.removeLongBurst(kDefaultFactorForWindowSize+3);
    remover.saveChunkList();

    remover.removeShortBurst();*/

    //------------------------------------------------------------------------
    // Get Tissue Area Writer ready
    //KESM::OrganizedTissueAreaWriter::Column column;
    KESM::TissueAreaWriter writer(reader.column);
    writer.writeFile(remover.destinationXMLFilename);
    writer.writeCSVFile(remover.destinationCSVFilename);

    return kKESM_OK; //a.exec();
}
