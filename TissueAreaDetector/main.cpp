#include <QtCore/QCoreApplication>
#include "TissueAreaDetector.h"

// ---------------------------------
// argv
// 0. Executable file name
// 1. Tissue area width
// 2. Template file name
// 3. Image file name
// 4. Options
//    s: [silence] no message displayed

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args = a.arguments();

    if(args.size() < 4)
    {
        std::cout << ">>> KESM Tissue Area Dectector ver 1.0 <<<" << std::endl;
        std::cout << "Detects the tissue area of an KESM raw image." << std::endl;

        QFileInfo pathInfo(args[0]);
        QString fileName(pathInfo.completeBaseName());
        std::cout << qPrintable(fileName )<< " TissueWidth TemplateFilename ImageFilename [s]" << std::endl;
        return -1;
    }
    int tissueWidth = args[1].toInt();
    QString templateName = args[2];

    KESM::TissueAreaDetector kesmTissueAreaDetector(tissueWidth, templateName);
    bool isSlience = false;
    if(args.size() >= 5 && args[4] == "s") {
        isSlience = true;
    }
    kesmTissueAreaDetector.setSilence(isSlience);
    int startX = kesmTissueAreaDetector.getStartX(args[3]);

    return startX; //a.exec();
}

