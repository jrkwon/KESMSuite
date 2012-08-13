#include <QFile>
#include <QTextStream>
#include <QDir>
#include "Merger.h"

typedef itk::ImageFileReader<InputImageType> ReaderType;
typedef itk::ImageFileWriter<OutputImageType> WriterType;
typedef itk::TileImageFilter<InputImageType, OutputImageType> TileFilterType;
typedef itk::InvertIntensityImageFilter<InputImageType> InvertIntensityImageFilterType;

KESM_NAMESPACE_START

Merger::Merger(QString imageInfoName, QString outputImageName, bool invertImage /*= false */)
{
    this->imageInfoName = QDir::fromNativeSeparators(imageInfoName);
    this->outputImageName = QDir::fromNativeSeparators(outputImageName);
    this->imageHeight = this->imageWidth = 0;
    if(kBackgroundColor == 0xFF)
        this->defaultPixelValue = itk::NumericTraits<InputImageType::PixelType>::max();
    else
        this->defaultPixelValue = itk::NumericTraits<InputImageType::PixelType>::Zero;
    this->invertImage = invertImage;
}

bool Merger::readInfo()
{
    QString folderName;
    int pos = imageInfoName.lastIndexOf('/');
    if(pos != -1)
        folderName = imageInfoName.left(pos);
    QString mergerInfoFileName = QString("%1/%2").arg(folderName).arg(kMergerInfoFile);

    QFile file(mergerInfoFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        //QByteArray line2 = in.readLine();
        QString line = in.readLine();
        if(line.startsWith("//") || line.trimmed().isEmpty())
            continue;

        QStringList items = line.split(":");
        if(items[0].compare(kTagNumberOfImagesToMerge, Qt::CaseInsensitive) == 0)
        {
            numberOfImageToMerge = items[1].toInt();
        }
        else if(items[0].compare(kTagBaseDir, Qt::CaseInsensitive) == 0)
        {
            QString partBaseDir = line.right(line.length()-strlen(kTagBaseDir)-1);
            baseDir = partBaseDir.trimmed();
            //baseDir = partBaseDir;//items[1];
        }
        else if(items[0].compare(kTagImageWidth, Qt::CaseInsensitive) == 0)
        {
            imageWidth = items[1].toInt();
        }
        else if(items[0].compare(kTagImageHeight, Qt::CaseInsensitive) == 0)
        {
            imageHeight = items[1].toInt();
        }
        else if(items[0].compare(kTagEdge, Qt::CaseInsensitive) == 0)
        {
            if(items[1].trimmed().compare("left", Qt::CaseInsensitive) == 0)
                edge = KESLeft;
            else
                edge = KESRight;
        }
    }

    if(imageWidth == 0 || imageHeight == 0)
    {
        std::cerr << "Width and Height are needed." << std::endl;
        return false;
    }
    return true;
}

bool Merger::readInfo4One()
{
    QFile file(imageInfoName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(line.startsWith("//") || line.trimmed().isEmpty())
            continue;

        QStringList items = line.split(":");
        bool ok;
        items[0].toInt(&ok); // check if it is a column number
        if(ok)
           imageNames.append(items[1].trimmed());
    }

    return true;
}

// create an image in memory in case there is no image with the z level.
void Merger::createImage(InputImageType::Pointer image)
{
    InputImageType::RegionType region;
    InputImageType::IndexType start;
    start[0] = 0;
    start[1] = 0;

    InputImageType::SizeType size;
    size[0] = imageWidth;
    size[1] = imageHeight;

    region.SetSize(size);
    region.SetIndex(start);

    image->SetRegions(region);
    image->Allocate();
    image->FillBuffer( defaultPixelValue);
}

bool Merger::execute(bool overWrite /*= false */)
{
    if(!readInfo())
        return false;

    readInfo4One();

    //QString outputFileName = QString("%1../%2").arg(baseDir).arg(outputImageName);

    if(overWrite == false)
    {
        // check if file exists
        QDir dir;
        if(dir.exists(outputImageName))
        {
            std::cout << "File exists: " << qPrintable(outputImageName) << std::endl;
            return false;
        }
    }

    TileFilterType::Pointer tiler = TileFilterType::New();
    itk::FixedArray< unsigned int, OutputImageDimension > layout;
    layout[0] = numberOfImageToMerge;
    layout[1] = 0;
    tiler->SetLayout( layout );

    ReaderType::Pointer reader = ReaderType::New();

    for(int i = 0; i < numberOfImageToMerge; i++)
    {
        InputImageType::Pointer inputImageTile;
        if(imageNames[i].isEmpty() == false)
        {
            QString inputImageTileName = QString("%1%2").arg(baseDir).arg(imageNames[i]);
            reader->SetFileName(inputImageTileName.toStdString());
            reader->UpdateLargestPossibleRegion();
            inputImageTile = reader->GetOutput();
        }
        else {
            inputImageTile = InputImageType::New();
            createImage(inputImageTile);
        }
        inputImageTile->DisconnectPipeline();
        if(edge == KESLeft)
            tiler->SetInput( numberOfImageToMerge-1-i, inputImageTile );
        else
            tiler->SetInput( i, inputImageTile );
    }

    tiler->SetDefaultPixelValue( defaultPixelValue);
    tiler->Update();

    InvertIntensityImageFilterType::Pointer invertIntensityFilter
       = InvertIntensityImageFilterType::New();
    if(invertImage)
    {
        invertIntensityFilter->SetInput(tiler->GetOutput());
        invertIntensityFilter->SetMaximum(kMaxPixelIntensity);
        invertIntensityFilter->Update();
    }

    // create the relighted image
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(outputImageName.toStdString());
    //writer->SetInput(outputImage );
    if(invertImage)
        writer->SetInput(invertIntensityFilter->GetOutput());
    else
        writer->SetInput(tiler->GetOutput());


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

    std::cout << "Merged: " << qPrintable(outputImageName) << std::endl;
    return true;
}


KESM_NAMESPACE_END
