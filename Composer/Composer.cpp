#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QtCore/qmath.h>
#include <QElapsedTimer>
#include <QDateTime>
#include "Composer.h"

typedef itk::ImageFileReader<InputImageType>    ReaderType;
typedef itk::ImageFileWriter<OutputImageType>   WriterType;
typedef itk::ImageLinearConstIteratorWithIndex<InputImageType>  InputItType;
typedef itk::ImageLinearIteratorWithIndex<InputImageType>       OutputItType;

KESM_NAMESPACE_START

Composer::Composer(QString baseFilePathName, QString outputPathName, int numberOfImages)
{
    this->logProcessingTime = false;

    this->baseFilePathName = QDir::fromNativeSeparators(baseFilePathName);
    this->outputPathName = QDir::fromNativeSeparators(outputPathName);
    QFileInfo pathInfo(this->baseFilePathName);
    this->baseFileName = pathInfo.fileName();
    this->baseDir = pathInfo.dir();
    this->basePathName = this->baseDir.path();
    this->numberOfImages = numberOfImages;

    int length = strlen(kIndexedImageFileNameFormat);
    this->baseIndexName = (this->baseFileName.left(length)).right(length-1);
    this->baseIndex = this->baseIndexName.toInt();
}

// create an image in memory in case there is no image with the z level.
void Composer::createImage(InputImageType::Pointer image)
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
    image->FillBuffer( itk::NumericTraits<PixelType>::max());
}

//#define kIntensityAtteunationFactor 0.05

const float kIntensityAtteunationFactor       = 0.1;
const int   kIntensityAttenuationInitPosition = 6;

/////////////////////////////////////////////////
// intensity attenuation parameters
// inensity
// 6 is good for initPoint when maximum depth is 40.
// the equation is basically y = x^2
// and normalize y into 0..1 to be used as a factor of the original intensity.

class IntensityAttenuation {
public:
    float rate;
    int initPoint;
    //int currentDepth;
public:
    //float factor() { return qPow(initPoint - depth*rate, 2)/qPow(initPoint, 2);}
    IntensityAttenuation(float rate, int initPoint) { this->rate = rate, this->initPoint = initPoint; }
    float factor(int depth) { return (initPoint - depth*rate)*(initPoint - depth*rate)/(initPoint*initPoint); }
};

bool Composer::execute(bool overWrite/* = false*/)
{
    ///////////////////////////////////
    // check the number of images
    // it should be greater than 1
    if(numberOfImages <= 1)
    {
        qDebug("numberOfImages should be greater than 1.");
        return false;
    }

    ///////////////////////////////////
    // check output file alreday exists.
    QString outputFilePathName = QString("%1/i%2%3").arg(outputPathName).arg(baseIndexName).arg(kSOURCE_IMAGE_FILE_EXT);
    // check if the file exists
    QFileInfo fileInfo(outputFilePathName);
    if(!overWrite && fileInfo.exists())
    {
        qDebug() << "File exists: " << outputFilePathName;
        qDebug("Creating the file aborted.");
        return false;
    }

    ///////////////////////////////////
    // Making a log file
    QTextStream logTextStream;
    QDateTime now = QDateTime::currentDateTime();
    QString logFileName = QString("%1-%2%3%4-%5%6%7%8.txt").arg(outputFilePathName)
            .arg(now.date().year()).arg(now.date().month()).arg(now.date().day())
            .arg(now.time().hour()).arg(now.time().minute()).arg(now.time().second()).arg(logTag);
    QFile logFile(logFileName);

    if(logProcessingTime) {
        if(!logFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Error while creating a log file: " << logFileName;
            return false;
        }
        logTextStream.setDevice(&logFile);
        // make a header
        logTextStream << "name\tread\tmerge\ttotal\n";
    }


    QElapsedTimer timerTotalTime;
    timerTotalTime.start();

    IntensityAttenuation attenuated(kIntensityAtteunationFactor, kIntensityAttenuationInitPosition);

    // get ready pointers
    ReaderType::Pointer baseReader = ReaderType::New();
    InputImageType::Pointer baseImage = InputImageType::New();
    ReaderType::Pointer layerReader = ReaderType::New();
    InputImageType::Pointer layerImage = InputImageType::New();
    OutputImageType::Pointer outputImage = OutputImageType::New();

    qDebug("Starting...");

    QElapsedTimer timerReadBaseFile;
    timerReadBaseFile.start();
    ///////////////////////////////////
    // init image info with base image
    QString baseFilePathName = this->baseFilePathName;
    baseReader->SetFileName(baseFilePathName.toStdString());
    baseReader->Update();
    imageWidth = baseReader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
    imageHeight = baseReader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
    baseImage = baseReader->GetOutput();

    qint64 elapsedReadBaseFile = timerReadBaseFile.elapsed();
    if(logProcessingTime)
        logTextStream << baseFileName << "\t" << elapsedReadBaseFile << "\t" << 0 << "\t" << elapsedReadBaseFile << "\n";


    //QString layerFilePath;

    for(int depth = 1; depth < numberOfImages /*&& ((baseIndex + depth) < maxIndex)*/; depth++)\
    {
        //////////////////////////
        // layer file
        // ------------
        // 1. find file starting with 'i<index number padding '0's.'
        QString layerFileNameFilter;
        QTextStream ts(&layerFileNameFilter);
        ts.setFieldWidth(indexedImagefileNameFormat.length());
        ts.setFieldAlignment(QTextStream::AlignRight);
        ts.setPadChar('0');
        ts << (baseIndex+depth);

        QStringList filters;
        filters << QString("i%1*%2").arg(layerFileNameFilter).arg(kSOURCE_IMAGE_FILE_EXT);
        baseDir.setNameFilters(filters);
        QStringList imageFileList = baseDir.entryList(QDir::Files);
        if(imageFileList.size() == 0) // no file.. error...
            return false;
        QString layerFileName = imageFileList[0];
        QString layerFilePathName = QString("%1/%2").arg(basePathName).arg(layerFileName);
        // ----------------
        // 2. read the file
        qDebug() << "- Start reading... " << layerFileName;
        QElapsedTimer timerFileRead;
        timerFileRead.start();
        layerReader->SetFileName(layerFilePathName.toStdString());
        layerReader->Update();
        layerImage = layerReader->GetOutput();

        qint64 elapsedReadTime =  timerFileRead.elapsed();
        qDebug("- Done (%d sec).", elapsedReadTime/1000);

        if(logProcessingTime)
            logTextStream << layerFileName << "\t" << elapsedReadTime;

        ////////////////////////////
        // output image
        createImage(outputImage);

        ////////////////////////////
        // init iterators
        InputItType baseIt(baseImage, baseImage->GetRequestedRegion());
        InputItType layerIt(layerImage, layerImage->GetRequestedRegion());

        OutputItType outputIt(outputImage, outputImage->GetRequestedRegion());

        baseIt.SetDirection(0);
        layerIt.SetDirection(0);
        outputIt.SetDirection(0);

        baseIt.GoToBegin();
        layerIt.GoToBegin();
        outputIt.GoToBegin();

        qDebug("= (%d/%d) Merging %d...", depth+1, numberOfImages, baseIndex+depth);
        QElapsedTimer timerMerge;
        timerMerge.start();

        ////////////////////////////
        // intensity attenuation rate
        // a = ((n - depth * r)^2) / n^2
        // where n = init point.
        // when the maximum deltph is around 40, n = 6 is good.

        PixelType basePixel, layerPixel, outputPixel;
        while(!baseIt.IsAtEnd())
        {
            while(!baseIt.IsAtEndOfLine())
            {
                basePixel = baseIt.Get();
                layerPixel = layerIt.Get() * attenuated.factor(depth);
                if(basePixel > layerPixel)
                    outputPixel = basePixel;
                else
                    outputPixel = layerPixel;

                outputIt.Set(outputPixel);

                ++baseIt;
                ++layerIt;
                ++outputIt;
            }
            baseIt.NextLine();
            layerIt.NextLine();
            outputIt.NextLine();
        }
        qint64 elapsedMergeTime = timerMerge.elapsed();
        qDebug("= Merged (%d sec).", elapsedMergeTime/1000);
        if(logProcessingTime)
            logTextStream << "\t" << elapsedMergeTime << "\t" << elapsedReadTime + elapsedMergeTime << "\n";

        // prepare the next image compose

        baseImage = NULL; // let ITK free allocated memory if it needed
        baseImage = outputImage;
        outputImage = OutputImageType::New();
        //layerImage = NULL; // let ITK free memory

    }
    qDebug("-- Creating output.");
    outputImage = NULL; // this will be release anyway since the progam ends soon.

    WriterType::Pointer writer = WriterType::New();
    writer->SetInput(baseImage); // In fact, baseImage has the last outputImage
    writer->SetFileName(outputFilePathName.toStdString());
    writer->Update();

    qDebug("Done.");

    qint64 elapsedTotalTime = timerTotalTime.elapsed();
    qDebug("Composing %d images took %d seconds.", numberOfImages, elapsedTotalTime/1000);
    if(logProcessingTime) {
        logTextStream << "Total\t\t\t" << elapsedTotalTime << "\n";
        logFile.close();
    }

    return true;
}

KESM_NAMESPACE_END
