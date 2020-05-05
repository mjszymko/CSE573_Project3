//
// Created by michael on 5/4/20.
//

#ifndef CSE573_PROJECT3_IMGPROCESSOR_H
#define CSE573_PROJECT3_IMGPROCESSOR_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <iostream>

class ImgProcessor {
private:
    cv::Ptr<cv::BackgroundSubtractor> pBackSubMOG2;
    cv::Ptr<cv::BackgroundSubtractor> pBackSubKNN;

    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier eyes_cascade;

public:
    bool BackSubInit();
    bool BackSubApply(std::string selection, cv::Mat frame, cv::Mat * fgmask);

    bool MorphologyApply(cv::Mat frame, cv::Mat *mask);

    bool CascadeClassifierInit(cv::Mat input, cv::Mat *output);
    bool CascadeClassifierUpdate(cv::Mat input, cv::Mat *ouptut);
};


#endif //CSE573_PROJECT3_IMGPROCESSOR_H
