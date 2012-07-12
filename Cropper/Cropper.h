#ifndef CROPPER_H
#define CROPPER_H

#include "../include/kesm.h"

#include <QObject>
#include <QRect>

KESM_NAMESPACE_START

class Cropper
{
public:
    bool crop(QString imageName, QRect &rect, QString croppedImageName, bool overWrite = true);

private:

};

KESM_NAMESPACE_END

#endif // CROPPER_H
