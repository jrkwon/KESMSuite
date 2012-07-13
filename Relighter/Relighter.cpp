#include <QFile>
#include "Relighter.h"

///////////////////////////
// itk
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"

typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;
///////////////////////////

KESM_NAMESPACE_START

Relighter::Relighter()
{
}

bool Relighter::execute(QString inImageFileName, QString outImageFileName, bool overWrite)
{
    if(QFile::exists(outImageFileName) && !overWrite)
        return false;

    ReaderType::Pointer reader = ReaderType::New();
    //const char * inputFilename  = inImageFileName.toStdString().c_str();

    reader->SetFileName(inImageFileName.toStdString());// inputFilename  );

    try
    {
        reader->Update(); // read an input image
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
    }

    ImageType::Pointer inputImage = ImageType::New();
    inputImage = reader->GetOutput();

    ////////////////////////////////////
    // create an output

    // prepare a region
    ImageType::IndexType start;
    start[0] = start[1] = 0;
    ImageType::SizeType size = inputImage->GetLargestPossibleRegion().GetSize();

    ImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    // prepare the output
    ImageType::Pointer outputImage = ImageType::New();
    outputImage->SetRegions(region);
    outputImage->Allocate();
    outputImage->FillBuffer(itk::NumericTraits<ImageType::PixelType>::max());//Zero);

    ConstIteratorType itInputX(inputImage, region);
    IteratorType      itOutputX(outputImage, region);
    relight(1, itInputX, itOutputX);

/*    ConstIteratorType itInputY(outputImage, region);
    IteratorType      itOutputY(outputImage, region);
    relight(1, itInputY, itOutputY);
*/
    // create the relighted image
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outImageFileName.toStdString());
    writer->SetInput(outputImage );

    try
    {
        writer->Update();
    }
    catch ( itk::ExceptionObject &err)
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
    }

    return true;
}

#define nILLUMINATION		150
#define nMAX_PIXEL_VALUE	255
#define nERROR_THRESHOLD	75
#define nERROR_FACTOR		0.8

void Relighter::relight(int direction, ConstIteratorType itInput, IteratorType itOutput)
{
    itInput.SetDirection(direction);
    itOutput.SetDirection(direction);

    for(itInput.GoToBegin(), itOutput.GoToBegin(); !itInput.IsAtEnd();
        itInput.NextLine(), itOutput.NextLine())
    {
        //int sum = 0;
        QVector<PixelType> pixelsInLine;

        while(!itInput.IsAtEndOfLine())
        {
            pixelsInLine << itInput.Get();  // it.Set() doesn't exist in the Const Iterator
            //if (m_TissueBlock.m_eRelightMethod == CTissueBlock::RM_AVERAGE_BASED)
            //    sum += itInput.Get();
            ++itInput;
        }
        qSort(pixelsInLine.begin(), pixelsInLine.end());
        PixelType median;
        //if (m_TissueBlock.m_eRelightMethod == CTissueBlock::RM_AVERAGE_BASED)
        //{
        //    // this 'median' is actually an average
        //    median = sum / pixelsInLine.count();
        //}
        //else
        //{
            median = pixelsInLine[pixelsInLine.size()/2];
        //}

        bool intensityError = false;

        if(median < nERROR_THRESHOLD)
            intensityError = true;

        for(itInput.GoToBeginOfLine(); !itInput.IsAtEndOfLine(); ++itInput, ++itOutput)
        {
            PixelType pixelValue = itInput.Get();
            if(intensityError == true)
            {
                if(pixelValue < median*nERROR_FACTOR) // (1-0.2)
                {
                    itOutput.Set(pixelValue); // use the original value
                    continue;
                }
            }
            double adjustedPixelValue = (pixelValue/(double)median) * nILLUMINATION;
            pixelValue = (adjustedPixelValue > (double)nMAX_PIXEL_VALUE)
                    ? (PixelType)nMAX_PIXEL_VALUE : (PixelType)adjustedPixelValue;
            itOutput.Set(pixelValue);
        }
        //QCoreApplication::processEvents();
    }
}

KESM_NAMESPACE_END
