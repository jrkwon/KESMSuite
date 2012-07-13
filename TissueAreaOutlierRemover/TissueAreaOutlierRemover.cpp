#include "TissueAreaOutlierRemover.h"
#include <QFile>
#include <QTextStream>
#include <iostream>

KESM_NAMESPACE_START

TissueAreaOutlierRemover::TissueAreaOutlierRemover(KesmData::Column *column)
{
    this->column = column;
}

void TissueAreaOutlierRemover::saveChunkList()
{
    if(chunks.size() == 0)
        return;

    QFile chunkListFile(chunkListFilename);
    if(chunkListFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&chunkListFile);
        for(int i = 0; i < chunks.size(); i++)
            out << chunks[i] << "\n";
        chunkListFile.close();
    }

}


void TissueAreaOutlierRemover::removeOutliers()
{
    int maxIndex = column->images.size();
    int sum;
    int i;

    //------------------------------------------------------------------------
    // 1st round: remove simple and short spikes
    for(i = 1; i < maxIndex-1; i++) {
        if(qAbs(column->images[i].startX - column->images[i-1].startX) > kImageChunkThreshold) {
            sum = (column->images[i].startX - column->images[i-1].startX);
            if(i+1 < maxIndex) {
                sum += (column->images[i+1].startX - column->images[i].startX);
            }
            // --------------
            // check outliers
            if(qAbs(sum) < kMaxRightEdgeDifference) {
                column->images[i].startX = column->images[i-1].startX;
                column->images[i].changed = true;
                //if(i+1 < maxIndex) {
                    column->images[i+1].startX = column->images[i].startX;
                    column->images[i+1].changed = true;
                //}
            }
            else if(((column->images[i+1].startX - column->images[i].startX)
                    *(column->images[i].startX - column->images[i-1].startX)) < 0){
                // different sign --> bump at the edge of a new chunk
                column->images[i].startX = column->images[i-1].startX;
                column->images[i].changed = true;
            }
        }
    }

    // 2nd round: remove more complex bumps
     for(i = 1; i < maxIndex; i++) {
        //std::cout << i << ":" << column->images[i].startX << std::endl;

        if(qAbs(column->images[i].startX - column->images[i-1].startX) > kImageChunkThreshold) {
            //int k;
            int outlierImages;
            int j;

            std::cout << i << ":misaligned candidate: " << column->images[i].startX << " diff: " << qAbs(column->images[i].startX - column->images[i-1].startX) << std::endl;

            for(sum = outlierImages = j = 0; i+j < maxIndex; j++) {
                if(qAbs(column->images[j+i].startX - column->images[j+i-1].startX) > kImageChunkThreshold) {
                    sum += (column->images[j+i].startX - column->images[j+i-1].startX);
                    outlierImages = 0; // reset it
                    //std::cout << i << ":accum sum: " << sum << " k:" << k << std::endl;
                }
                else {
                    if(sum != 0)
                        sum += (column->images[j+i].startX - column->images[j+i-1].startX);

                    // tolerate a few consecutive wrong images
                    if(outlierImages++ >= kMaxOutlierImages)
                        break;
                }
            }
            // --------------
            // check outliers
            if(qAbs(sum) < kMaxRightEdgeDifference) {
                for(int m = 0; m < j; m++) {
                    std::cout << m << ":reset: i+m: " << i+m << ", org:" <<  column->images[i+m].startX << ", new: " << column->images[i-1].startX << std::endl;
                    column->images[i+m].startX = column->images[i-1].startX;
                    column->images[i+m].changed = true;
                }
            }
        }
    }
}

void TissueAreaOutlierRemover::scanChunks()
{
    int maxIndex = column->images.size();
    int i;

    chunks.append(0); // at least, there is one chunk that starts from 0.
    for(i = 1; i < maxIndex-1; i++) {
        if(qAbs(column->images[i].startX - column->images[i-1].startX) > kImageChunkThreshold) {
            chunks.append(i);
        }
    }
    chunks.append(maxIndex); // add last image index
}

void TissueAreaOutlierRemover::smoothingChunks()
{
    int maxIndex = column->images.size();
    int i, c;
    int count;
    long sum;
    QList<int> average;

    scanChunks();

    // get the average of x start from each range
    for(i = 0, c = 1; c < chunks.size(); c++) {
        sum = 0;
        for(count = 0; i < chunks[c] && i < maxIndex; i++, count++) {
            sum += column->images[i].startX;
        }
        if(count != 0 )
            average.append(sum/count);
        else
            average.append(sum); // just in case
    }

    for(i = 0, c = 1; c < chunks.size(); c++) {
        for(; i < chunks[c] && i < maxIndex; i++)
            column->images[i].newStartX = average[c-1];
    }
}

KESM_NAMESPACE_END
