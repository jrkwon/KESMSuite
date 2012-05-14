#ifndef TISSUEAREAOUTLIERREMOVER_H
#define TISSUEAREAOUTLIERREMOVER_H

#include <QString>
#include <QList>

#include "../include/kesm.h"

KESM_NAMESPACE_START

class TissueAreaOutlierRemover
{
public:
    TissueAreaOutlierRemover(KesmData::Column *column);

public:
    void saveChunkList();
    void removeShortBurst();
    void removeLongBurst(int factorForWindowSize);

public:
    QString sourceXMLFilename;
    QString destinationXMLFilename;
    QString destinationCSVFilename;
    QString chunkListFilename;

private:
    KesmData::Column *column;
    QList<int> chunks;
};

KESM_NAMESPACE_END

#endif // TISSUEAREAOUTLIERREMOVER_H
