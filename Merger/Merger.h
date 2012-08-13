#ifndef MERGER_H
#define MERGER_H

#include "../include/kesm.h"
#include <QObject>
#include <QString>
#include <QStringList>

////////////////////
// info file format
// NumberOfImagesToMerge:	<colums>
// BaseDir:	<base dir name>
// Width:   <width>
// Height:  <height>
// <idx number>:   <image file name>
// <idx number>:   <image file name>

/* --- example ---
NumberOfImagesToMerge:	6
BaseDir:	K:/Projects/KESMData/2008-01-23 Whole mouse brain vasculature/Relighted/
Width:  2400
Height: 12000
0:	0/20080415_135412_x148.0967y31.1286z10.5830_t0.001000_v17.4763.jpg
1:
2:	2/20080411_170844_x148.0967y28.1286z10.5830_t0.001000_v19.1768.jpg
3:	3/20080411_170138_x148.0967y26.6286z10.5830_t0.001000_v19.6109.jpg
4:	4/0080411_165122_x148.0967y25.1286z10.5830_t0.001000_v17.5233.jpg
5:
 --------------------- */

#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkTileImageFilter.h"
#include "itkInvertIntensityImageFilter.h"

const unsigned int InputImageDimension = 2;
const unsigned int OutputImageDimension = 2;
typedef unsigned char PixelType;
typedef itk::Image<PixelType, InputImageDimension> InputImageType;
typedef itk::Image<PixelType, OutputImageDimension> OutputImageType;

KESM_NAMESPACE_START

//#define kTagNumberOfImagesToMerge   "NumberOfImagesToMerge"
//#define kTagImageWidth              "ImageWidth"
//#define kTagImageHeight             "ImageHeight"
//#define kTagBaseDir                 "BaseDir"
//#define kTagEdge                    "Edge"
//#define kLeft                       "Left"
//#define kRight                      "Right"

class Merger
{
public:
    Merger(QString imageInfoName, QString outputImageName, bool invertImage = false);
    bool execute(bool overWrite = false);

protected:
    bool readInfo();
    bool readInfo4One();
    void createImage(InputImageType::Pointer image);

    QString imageInfoName;
    QString outputImageName;

private:
    int numberOfImageToMerge;
    int imageWidth;
    int imageHeight;
    KnifeEdgeSide edge;
    QString baseDir;
    bool invertImage;

    QList<QString> imageNames;

    PixelType defaultPixelValue;
};

KESM_NAMESPACE_END

#endif // MERGER_H
