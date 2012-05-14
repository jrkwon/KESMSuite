#ifndef TISSUEAREADETECTOR_H
#define TISSUEAREADETECTOR_H

#define KESM_NAMESPACE_START namespace KESM {
#define KESM_NAMESPACE_END   }

#include <QtDebug>
#include <QFileInfo>
#include <QStringList>
#include <QSharedMemory>
#include <QBuffer>

///////////////////////////
// itk
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkImage.h"

const unsigned int g_nDimension = 2;

typedef unsigned char PixelType;
typedef itk::Image<PixelType, g_nDimension> ImageType;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;
typedef itk::RegionOfInterestImageFilter< ImageType,
                                          ImageType > FilterType;
typedef itk::ImageLinearConstIteratorWithIndex<ImageType> ConstIteratorType;
typedef itk::ImageLinearIteratorWithIndex<ImageType>      IteratorType;


KESM_NAMESPACE_START

struct TissueRightEdgeTemplate {
    QString name;
    int width;
    int height;
};

class TissueAreaDetector {
public:
    TissueAreaDetector(int tissueWidth, QString templateName);

    void reLight(int direction,
                   itk::ImageLinearConstIteratorWithIndex<ImageType> inputIt,
                   itk::ImageLinearIteratorWithIndex<ImageType> outputIt);
    int getStartX(QString fileName);
    int sumOfAbsoluteDifferences(
            itk::ImageLinearConstIteratorWithIndex<ImageType> searchIt,
            itk::ImageLinearConstIteratorWithIndex<ImageType> templateIt);
    void showProgress();
    void saveTissueArea();
    void setSilence(bool isSilence);
    void displayMessage(QString message);

private:
    //    KesmRawImage kesmRawImage;
    TissueRightEdgeTemplate tissueRightEdgeTemplate;
    int tissueWidth;
    int startX;
    bool isSilence;
    QSharedMemory sharedMemory;
};

KESM_NAMESPACE_END

#endif // TISSUEAREADETECTOR_H
