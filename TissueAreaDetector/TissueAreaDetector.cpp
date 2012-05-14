#include "TissueAreaDetector.h"

KESM_NAMESPACE_START

TissueAreaDetector::TissueAreaDetector(int tissueWidth, QString templateName)
{
    this->tissueWidth = tissueWidth;
    this->tissueRightEdgeTemplate.name = templateName;

    this->startX = -1;
    this->isSilence = false;
}

int TissueAreaDetector::sumOfAbsoluteDifferences(
        itk::ImageLinearConstIteratorWithIndex<ImageType> searchIt,
        itk::ImageLinearConstIteratorWithIndex<ImageType> templateIt)
{
    searchIt.SetDirection(0);
    templateIt.SetDirection(0);

    PixelType searchPixelValue, templatePixelValue;
    int sad = 0;

    for(searchIt.GoToBegin(), templateIt.GoToBegin(); !searchIt.IsAtEnd();
        searchIt.NextLine(), templateIt.NextLine())
    {
        for(searchIt.GoToBeginOfLine(); !searchIt.IsAtEndOfLine(); ++searchIt, ++templateIt)
        {
            searchPixelValue = searchIt.Get();
            templatePixelValue = templateIt.Get();
            sad += qAbs((int)searchPixelValue-(int)templatePixelValue);
        }
        //QCoreApplication::processEvents();
    }
    return sad;
}

#define kMaxIllumination		0xFF    //
#define kMinIllumination                0x67    //
#define kMaxPixelValue                  0xFF

// uncomment this, should you want to save re-lighted region images
// #define kReLightedFileSave


void TissueAreaDetector::reLight(int direction,
                                   itk::ImageLinearConstIteratorWithIndex<ImageType> inputIt,
                                   itk::ImageLinearIteratorWithIndex<ImageType> outputIt)
{
    inputIt.SetDirection(direction);
    outputIt.SetDirection(direction);

    for(inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd();
        inputIt.NextLine(), outputIt.NextLine())
    {
        QVector<PixelType> pixelsInLine;
        double sumOfALine = 0;
        while( !inputIt.IsAtEndOfLine() )
        {
            sumOfALine += inputIt.Get();
            pixelsInLine << inputIt.Get();  // it.Set() doesn't exist in the Const Iterator
            ++inputIt;
        }
        //qSort(pixelsInLine.begin(), pixelsInLine.end());
        //PixelType Median = pixelsInLine[pixelsInLine.size()/2];
        PixelType average = sumOfALine/pixelsInLine.size();

//        bool bIntensityError = false;

//        if(Median < nERROR_THRESHOLD)
//            bIntensityError = true;

        for(inputIt.GoToBeginOfLine(); !inputIt.IsAtEndOfLine(); ++inputIt, ++outputIt)
        {
            PixelType pixelValue = inputIt.Get();
//            if(bIntensityError == true)
//            {
//                if(PixelValue < Median*nERROR_FACTOR) // (1-0.2)
//                {
//                    outputIt.Set(PixelValue); // use the original value
//                    continue;
//                }
//            }
            double adjustPixelValue = (pixelValue/(double)average) * kMaxIllumination;
//            pixelValue = (adjustPixelValue > (double)nMAX_PIXEL_VALUE)
            if(adjustPixelValue > (double)kMaxIllumination)
                pixelValue = (PixelType)kMaxIllumination;
            else if(adjustPixelValue < (double)kMinIllumination)
                pixelValue = (PixelType)0;
            else
                pixelValue = (PixelType)adjustPixelValue;

//            pixelValue = (adjustPixelValue > (double)kIllumination)
//                        ? (PixelType)kIllumination : (PixelType)adjustPixelValue;
            outputIt.Set(pixelValue);
        }
        //QCoreApplication::processEvents();
    }
}

int TissueAreaDetector::getStartX(QString fileName)
{
    //    if(QFile::exists(strSrcImage) != true)
    //    {
    //        qDebug() << "Error: " << qPrintable(fileName) << "does not exist.";
    //    }

    ///////////////////////////////
    // read the template image file

    ReaderType::Pointer templateReader = ReaderType::New();
    templateReader->SetFileName(tissueRightEdgeTemplate.name.toStdString().c_str());
    try
    {
        templateReader->Update();
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    // get template image width and height;
    ImageType::ConstPointer templateImage = templateReader->GetOutput();
    ImageType::SizeType templateImageSize = templateImage->GetLargestPossibleRegion().GetSize();
    tissueRightEdgeTemplate.width = templateImageSize[0];
    tissueRightEdgeTemplate.height = templateImageSize[1];

    //////////////////////////////////////////////////
    // TODO: Normalize intensity level of the template image
    //

    ///////////////////////////////////////////
    // Read the image to detect a tissue region

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(fileName.toStdString().c_str());

    try
    {
        reader->Update();
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }

    ImageType::ConstPointer inputImage = reader->GetOutput();
    ImageType::SizeType imageSize = inputImage->GetLargestPossibleRegion().GetSize();
    int imageWidth = imageSize[0];

    ImageType::SizeType size;
    size[0] = tissueRightEdgeTemplate.width;
    size[1] = tissueRightEdgeTemplate.height;
    ImageType::IndexType start;

    int minSad, minSadX;
    int sad;
    int rightEdgeX;
     int x, y;

    minSadX = 0;
    minSad = -1;

    for(y = 0, x = imageWidth-tissueRightEdgeTemplate.width; x > tissueWidth-tissueRightEdgeTemplate.width/2; x--)
    {
        ///////////////////////////////////////
        // read a region from the search image
        start[0] = x, start[1] = y;

        ImageType::RegionType desiredRegion;
        desiredRegion.SetSize(size);
        desiredRegion.SetIndex(start);
        FilterType::Pointer filter = FilterType::New();
        filter->SetRegionOfInterest(desiredRegion);
        filter->SetInput(reader->GetOutput());

        try
        {
            filter->Update();
        }
        catch( itk::ExceptionObject & err )
        {
            std::cerr << "ExceptionObject caught !" << std::endl;
            std::cerr << err << std::endl;
            return EXIT_FAILURE;
        }

        //////////////////////////////////////////////////
        // Normalize intensity level of the region
        //////////////////////////////////////////////////
        // 1. prepare the output
        ImageType::Pointer outputImage = ImageType::New();
        outputImage->SetRegions( filter->GetOutput()->GetRequestedRegion() );
        //outputImage->CopyInformation(pReader->GetOutput());
        outputImage->Allocate();
        // 2. fill buffer with initial value
        ImageType::PixelType  initialValue = kMaxPixelValue;
        outputImage->FillBuffer( initialValue );
        // 3. re-light
        ConstIteratorType inputItX(filter->GetOutput(), filter->GetOutput()->GetRequestedRegion());
        IteratorType      outputItX(outputImage, filter->GetOutput()->GetRequestedRegion());
        reLight(0, inputItX, outputItX);

        //ConstIteratorType inputItY(outputImage, filter->GetOutput()->GetRequestedRegion());
        //IteratorType      outputItY(outputImage, filter->GetOutput()->GetRequestedRegion());
        //doRelight(1, inputItY, outputItY);
        /////////////////////////////////////



#ifdef kReLightedFileSave
        ////////////////////
        // relighting debug
        WriterType::Pointer writer = WriterType::New();
        static int i = 0;
        QString tempFilename = QString("%1.jpg").arg(i++);
        writer->SetFileName(tempFilename.toStdString().c_str());
        //writer->SetInput(filter->GetOutput());
        writer->SetInput(outputImage );

        try
        {
            writer->Update();
        }
        catch ( itk::ExceptionObject &err)
        {
            std::cerr << "ExceptionObject caught !" << std::endl;
            std::cerr << err << std::endl;
            return EXIT_FAILURE;
        }
#endif


        //////////////////////////////////////////
        // Calculate Sum of Absolute Differences
        //////////////////////////////////////////
        //ConstIteratorType searchIt(filter->GetOutput(), filter->GetOutput()->GetRequestedRegion());
        ConstIteratorType searchIt(outputImage, outputImage->GetRequestedRegion());
        ConstIteratorType templateIt(templateReader->GetOutput(), templateReader->GetOutput()->GetRequestedRegion());

        sad = sumOfAbsoluteDifferences(searchIt, templateIt);
        if((minSad == -1) || (sad < minSad))
        {
            minSad = sad;
            minSadX = x;
            //            qDebug() << "minSad: " << minSad << "minSadX: " << minSadX;
        }

        showProgress();
    }

    rightEdgeX = minSadX +  tissueRightEdgeTemplate.width/2;
    startX = rightEdgeX - tissueWidth;

    displayMessage(QString("startX: %1").arg(startX));

    return startX;
}

void TissueAreaDetector::showProgress()
{
    if(isSilence == true)
        return;

    char verb[] = "-\\|/";
    static int i = 0;

    if(verb[i] == NULL)
        i = 0;
    std::cout << verb[i++] << "\r";
}

void TissueAreaDetector::saveTissueArea()
{
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    QDataStream out(&buffer);
    out << startX;
    int size = buffer.size();

    if (!sharedMemory.create(size)) {
        qDebug() << "Error: Unable to create shared memory segment.";
        return;
    }
    sharedMemory.lock();
    char *to = (char*)sharedMemory.data();
    const char *from = buffer.data().data();
    memcpy(to, from, qMin(sharedMemory.size(), size));
    sharedMemory.unlock();
}

void TissueAreaDetector::setSilence(bool isSilence)
{
    this->isSilence = isSilence;
}

void TissueAreaDetector::displayMessage(QString message)
{
    if(isSilence == false)
        std::cout << qPrintable(message) << std::endl;
}

KESM_NAMESPACE_END
