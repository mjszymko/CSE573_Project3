//
// Created by michael on 5/4/20.
//

#include "ImgProcessor.h"
#include <vector>



bool ImgProcessor::BackSubInit()
{
    // default is history = 500, varThreshold = 16, detectShadows = 1
    pBackSubMOG2 = cv::createBackgroundSubtractorMOG2(100, 16, 0);

    pBackSubKNN = cv::createBackgroundSubtractorKNN();

    return true;
}

bool ImgProcessor::BackSubApply(std::string selection, cv::Mat frame, cv::Mat * fgmask)
{
    if(selection == "MOG2")
    {
        pBackSubMOG2->apply(frame,*fgmask);
        return true;
    }
    else if(selection == "KNN")
    {
        pBackSubKNN->apply(frame,*fgmask);
        return true;
    }
    else
    {
        return false;
    }
}

bool ImgProcessor::MorphologyApply(cv::Mat frame, cv::Mat *mask)
{
    cv::Mat element = getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( 3,3), cv::Point( 1, 1 ) );
    morphologyEx( frame, *mask, cv::MORPH_OPEN, element );

    return true;
}