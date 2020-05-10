//
// Created by michael on 5/4/20.
//

#ifndef CSE573_PROJECT3_CAMERAFEED_H
#define CSE573_PROJECT3_CAMERAFEED_H

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>

#include "mynteye/api/api.h"
#include "mynteye/logger.h"
#include <opencv2/imgproc/imgproc.hpp>

MYNTEYE_USE_NAMESPACE

class CameraFeed
{
private:
    std::shared_ptr<API> api;

public:
    bool Init();
    bool GetLeft(cv::Mat * frame);
    bool GetLeftRectified(cv::Mat * frame);
    bool GetRightRectified(cv::Mat * frame);
    bool GetDepth(cv::Mat * frame);
    bool Terminate();
    bool GetDisparityNormalized(cv::Mat *frame);
};


#endif //CSE573_PROJECT3_CAMERAFEED_H
