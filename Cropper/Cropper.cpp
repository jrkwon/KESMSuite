#include <QFile>
#include <QRect>
#include "Cropper.h"

///////////////////////////
// itk
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkCropImageFilter.h"
#include "itkImage.h"

typedef unsigned char PixelType;
typedef itk::Image<PixelType, 2> ImageType;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;
typedef itk::CropImageFilter<ImageType, ImageType> FilterType;
typedef itk::ImageLinearConstIteratorWithIndex<ImageType> ConstIteratorType;
typedef itk::ImageLinearIteratorWithIndex<ImageType>      IteratorType;
///////////////////////////

KESM_NAMESPACE_START

bool Cropper::crop(QString imageName, QRect &rect,  QString croppedImageName, bool overWrite)
{
    if(QFile::exists(croppedImageName) && !overWrite)
    {
        return false;
    }

    //////////////
    // reader
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( imageName.toStdString()  );
    try
    {
        reader->Update();
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return false; // EXIT_FAILURE;
    }

    ////////////////////////////////////////////
    // !!! LowerBoundar is the top left.
    // !!! UpperBoundary is the right bottom.
    // This is very confusing. This may be a bug.

    //////////////////
    // get crop size
    ImageType::SizeType cropSizeUpper, cropSizeLower;
    int x, y, width, height;
    rect.getRect(&x, &y, &width, &height);
    cropSizeLower[0] = x, cropSizeLower[1] = y;
    // get input image size
    ImageType::SizeType imageSize = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
    cropSizeUpper[0] = imageSize[0]-(width+x);
    if(height == -1) // use the image height
        cropSizeUpper[1] = 0;
    else
        cropSizeUpper[1] = imageSize[1]-(height+y);

    //////////////
    // filter
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(reader->GetOutput());

    filter->SetUpperBoundaryCropSize(cropSizeUpper);
    filter->SetLowerBoundaryCropSize(cropSizeLower);

    try
    {
        filter->Update();
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return false; // EXIT_FAILURE;
    }

    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(croppedImageName.toStdString());
    writer->SetInput( filter->GetOutput() );

    try
    {
        writer->Update();
    }
    catch ( itk::ExceptionObject &err)
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return false;
    }

    return true;
}

KESM_NAMESPACE_END
