#ifndef RELIGHTER_H
#define RELIGHTER_H

#include "../include/kesm.h"

///////////////////////////
// itk
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"

typedef unsigned char PixelType;
typedef itk::Image<PixelType, 2> ImageType;
typedef itk::ImageLinearConstIteratorWithIndex<ImageType> ConstIteratorType;
typedef itk::ImageLinearIteratorWithIndex<ImageType>      IteratorType;
///////////////////////////

KESM_NAMESPACE_START

class Relighter
{
public:
    Relighter();

    bool execute(QString inImageFileName, QString outImageFileName, bool overWrite = false);

private:
    void relight(int direction, ConstIteratorType InputIt, IteratorType OutputIt);
};

KESM_NAMESPACE_END

#endif // RELIGHTER_H
