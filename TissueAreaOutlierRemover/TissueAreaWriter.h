#ifndef TISSUEAREAWRITER_H
#define TISSUEAREAWRITER_H

#include "../include/kesm.h"

#include <QObject>
#include <QString>
#include <QVector>
#include <QProcess>
#include <QXmlStreamWriter>

KESM_NAMESPACE_START

class TissueAreaWriter {
public:
    TissueAreaWriter(KesmData::Column &column);
    bool writeFile(const QString &fileName);
    bool writeCSVFile(const QString &fileName);
    KesmData::Column* getColumn() { return &column; }

private:
    void write();

    QXmlStreamWriter writer;
    KesmData::Column column;
};


KESM_NAMESPACE_END

#endif // TissueAreaWriter_H
