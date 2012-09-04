#ifndef COMPOSER_H
#define COMPOSER_H

#include "../include/kesm.h"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>

#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"

const unsigned int InputImageDimension = 2;
const unsigned int OutputImageDimension = 2;
typedef unsigned char PixelType;
typedef itk::Image<PixelType, InputImageDimension> InputImageType;
typedef itk::Image<PixelType, OutputImageDimension> OutputImageType;

KESM_NAMESPACE_START

///////////////////////////////////////////////////////////
// Path and File Name Specifications
//
// FileName:        file name without having a path
// PathName:        path name without having a file name
// FilePathName:    file name including a path
// DirName:         directory name
// Dir:             qDir object


/////////////////////////
/////////////////////////
//
// Base file name format <= indexed image file format
// iNNNNNNzZ.ZZZZ.jpg
// NNNNNN: image index
// Z.ZZZZ: z axis position in the KESM
//

//const QString indexedImagefileNameFormat = "NNNNN";
//#define kIndexedImageFileNameFormat     "iNNNNN" // zZ.ZZZZ.jpg"
//#define kZPosFileNameFormat             "Z.ZZZZ"

class Composer
{
public:
    Composer(QString baseFilePathName, QString outputPathName, int numberOfImages);
    bool execute(bool overWrite = false);

protected:
    void createImage(InputImageType::Pointer image);

private:
    QString outputPathName;     // output dir
    QString baseFileName;       // file name only
    QString basePathName;       // full path without file name. no ending '/'
    QDir baseDir;               // QDir object for baseDirName
    QString baseFilePathName;   // full path
    int numberOfImages;

    int baseIndex;
    QString baseIndexName;      // this is used for output filename

    // image info
    int imageWidth;
    int imageHeight;

};

KESM_NAMESPACE_END

#endif // COMPOSER_H
