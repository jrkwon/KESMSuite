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
    struct RawImage {
        QString name;
        int startX;     // tissue start point
//        int orgStartX;
//        bool adjust;
        bool valid;
    };

    struct Column {
        int id;         // column number
        int width;      // tissue area width

        QVector<RawImage> images;
    };

public:
    TissueAreaWriter(Column &column);
    bool writeFile(const QString &fileName);
    bool writeCSVFile(const QString &fileName);
    Column* getColumn() { return &column; }

private:
    void write();

    QXmlStreamWriter writer;
    Column column;
};


KESM_NAMESPACE_END

#endif // TISSUEAREAWRITER_H
