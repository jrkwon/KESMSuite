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


void TissueAreaOutlierRemover::removeShortBurst()
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
#if 0
            outlierImages = j = 0;
            while(k+i+j < maxIndex) {
                if(qAbs(column->images[k+j+i].startX - column->images[k+j+i-1].startX) > kImageChunkThreshold) {
                    sum += (column->images[k+j+i].startX - column->images[k+j+i-1].startX);
                    k++;
                    outlierImages = 0; // reset it
                    std::cout << i << ":accum sum: " << sum << " k:" << k << std::endl;
                }
                else {
                    if(k != 0)
                        sum += (column->images[k+j+i].startX - column->images[k+j+i-1].startX);

                    j++;
                    // tolerate a few consecutive wrong images
                    if(outlierImages++ >= kMaxOutlierImages)
                        break;
                }
            }
            // --------------
            // check outliers
            if(qAbs(sum) < kMaxRightEdgeDifference) {
                for(int m = 0; m < k+j; m++) {
                    std::cout << m << ":reset: i+m: " << i+m << ", org:" <<  column->images[i+m].startX << ", new: " << column->images[i-1].startX << std::endl;
                    column->images[i+m].startX = column->images[i-1].startX;
                    column->images[i+m].changed = true;
                }
            }
            else if (i != maxIndex-1){
                // identify a new chunk
                chunks.append(i);
                std::cout << i << ": new chunk: startX: " << column->images[i].startX << std::endl;
            }
#endif
        }
    }
}

void TissueAreaOutlierRemover::removeLongBurst(int factorForWindowSize)
{
    int maxIndex = column->images.size();

    for(int i = 1; i < maxIndex; i++) {
        std::cout << i << ":" << column->images[i].startX << std::endl;
        if(qAbs(column->images[i].startX - column->images[i-1].startX) > kImageChunkThreshold) {
            std::cout << i << ":misaligned candidate: " << column->images[i].startX << " diff: " << qAbs(column->images[i].startX - column->images[i-1].startX) << std::endl;

            int sum = 0;
            int k;
            int maxConsecutiveWrongImages = 0;
            // ------------------
            // sum of differences
//            for(k = 0; (k+i < maxIndex) && qAbs(column->images[k+i].startX - column->images[k+i-1].startX) > kImageChunkThreshold; k++) {

            for(k = 0; (k+i < maxIndex); maxConsecutiveWrongImages++) {
                sum += (column->images[k+i].startX - column->images[k+i-1].startX);
                k++;
                std::cout << i << ":accum sum: " << sum << " k:" << k << std::endl;
                // tolerate a few consecutive wrong images
                if(maxConsecutiveWrongImages >= kMaxConsecutiveWrongImages*factorForWindowSize)
                    break;
            }
            // --------------
            // check outliers
            if(qAbs(sum) < kMaxRightEdgeDifference) {
                for(int m = 0; m < k; m++) {
                    std::cout << m << ":reset: i+m: " << i+m << ", org:" <<  column->images[i+m].startX << ", new: " << column->images[i-1].startX << std::endl;
                    column->images[i+m].startX = column->images[i-1].startX;
                    column->images[i+m].changed = true;
                }
            }
        }
    }
}


/* ---------------------------------
void TissueAreaOutlierRemover::removeShortBurst()
{
    int maxIndex = column->images.size();

    //------------------------------------------------------------------------
    // 1st round: Remove short bursts
    for(int i = 1; i < maxIndex; i++) {
        std::cout << i << ":" << column->images[i].startX << std::endl;

        if(qAbs(column->images[i].startX - column->images[i-1].startX) > kImageChunkThreshold) {

            std::cout << i << ":misaligned candidate: " << column->images[i].startX << " diff: " << qAbs(column->images[i].startX - column->images[i-1].startX) << std::endl;

            int sum = 0;
            int k;
            int maxOutlierImages;
            // ------------------
            // sum of differences
//            for(k = 0; (k+i < maxIndex) && qAbs(column->images[k+i].startX - column->images[k+i-1].startX) > kImageChunkThreshold; k++) {

            for(maxOutlierImages = k = 0; maxOutlierImages+k+i < maxIndex; maxOutlierImages++) {
                if(qAbs(column->images[maxOutlierImages+k+i].startX - column->images[maxOutlierImages+k+i-1].startX) > kImageChunkThreshold) {
                    sum += (column->images[k+i].startX - column->images[k+i-1].startX);
                    k++;
                    std::cout << i << ":accum sum: " << sum << " k:" << k << std::endl;
                }
                else {
                    // tolerate a few consecutive wrong images
                    if(maxOutlierImages >= kMaxOutlierImages)
                        break;
                }
            }
            // --------------
            // check outliers
            if(qAbs(sum) < kMaxRightEdgeDifference) {
                for(int m = 0; m < k; m++) {
                    std::cout << m << ":reset: i+m: " << i+m << ", org:" <<  column->images[i+m].startX << ", new: " << column->images[i-1].startX << std::endl;
                    column->images[i+m].startX = column->images[i-1].startX;
                    column->images[i+m].changed = true;
                }
            }
            else if (i != maxIndex-1){
                // identify a new chunk
                chunks.append(i);
                std::cout << i << ": new chunk: startX: " << column->images[i].startX << std::endl;
            }
        }
    }
}

void TissueAreaOutlierRemover::removeLongBurst(int factorForWindowSize)
{
    int maxIndex = column->images.size();

    for(int i = 1; i < maxIndex; i++) {
        std::cout << i << ":" << column->images[i].startX << std::endl;
        if(qAbs(column->images[i].startX - column->images[i-1].startX) > kImageChunkThreshold) {
            std::cout << i << ":misaligned candidate: " << column->images[i].startX << " diff: " << qAbs(column->images[i].startX - column->images[i-1].startX) << std::endl;

            int sum = 0;
            int k;
            int maxConsecutiveWrongImages = 0;
            // ------------------
            // sum of differences
//            for(k = 0; (k+i < maxIndex) && qAbs(column->images[k+i].startX - column->images[k+i-1].startX) > kImageChunkThreshold; k++) {

            for(k = 0; (k+i < maxIndex); maxConsecutiveWrongImages++) {
                sum += (column->images[k+i].startX - column->images[k+i-1].startX);
                k++;
                std::cout << i << ":accum sum: " << sum << " k:" << k << std::endl;
                // tolerate a few consecutive wrong images
                if(maxConsecutiveWrongImages >= kMaxConsecutiveWrongImages*factorForWindowSize)
                    break;
            }
            // --------------
            // check outliers
            if(qAbs(sum) < kMaxRightEdgeDifference) {
                for(int m = 0; m < k; m++) {
                    std::cout << m << ":reset: i+m: " << i+m << ", org:" <<  column->images[i+m].startX << ", new: " << column->images[i-1].startX << std::endl;
                    column->images[i+m].startX = column->images[i-1].startX;
                    column->images[i+m].changed = true;
                }
            }
        }
    }
}
------------------------------------------- */

KESM_NAMESPACE_END
