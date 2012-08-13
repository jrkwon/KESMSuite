#ifndef GENMERGEIINFO_H
#define GENMERGEIINFO_H

#include "../include/kesm.h"
#include "../RawDataReader/RawDataReader.h"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>

KESM_NAMESPACE_START

class GenMergerInfo
{
public:
    GenMergerInfo(QString inputFolderName, QString outputFolderName);
    bool collectFileInfo();
    void createMergerInfo();
    void execute();

protected:
    struct FilesInColumn
    {
        int columnNumer;
        QStringList files;
    };
    QString inputFolderName;
    QString outputFolderName;

    QList<FilesInColumn> columnFileList;
    RawDataReader rawDataReader;

    struct MergerInfo {
        int numberOfImagesToMerge; // means number of columns anyway
        QString baseDir;
        int imageWidth;
        int imageHeight;
        KnifeEdgeSide edge;
    };
    MergerInfo mergerInfo;
};

KESM_NAMESPACE_END

#endif // GENMERGEIINFO_H
