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
#include <vector>

class ImgProcessor {
private:
    std::string tbn = "control_panel";

    // trackbar variables
    int morphKernelSz;
    int morphTypes;
    int retrievalMode;
    int contourApproxMode;
    int cannyThreshLow;
    int cannyThreshHigh;
    int maxContours;
    int erosionMask;
    int learningRate;
    int apertureSize;


    cv::Ptr<cv::BackgroundSubtractor> pBackSubMOG2;
    cv::Ptr<cv::BackgroundSubtractor> pBackSubKNN;

    cv::CascadeClassifier closed_palm_cascade;
    cv::CascadeClassifier fist_cascade;
    cv::CascadeClassifier closed_frontal_palm_cascade;
    cv::CascadeClassifier aGest_cascade;
    cv::CascadeClassifier palm_cascade;
    cv::CascadeClassifier open_palm_cascade;

    std::vector<cv::CascadeClassifier> classifiers;


public:
    bool CreateTrackbar();

    bool BackSubInit();
    bool BackSubApply(std::string selection, cv::Mat frame, cv::Mat * fgmask);

    bool MorphologyApply(cv::Mat frame, cv::Mat *mask);

    bool CascadeClassifierInit(cv::Mat input, cv::Mat *output);
    bool CascadeClassifierUpdate(cv::Mat input, cv::Mat *ouptut);

    bool HaarClassifierInit();
    bool DetectHand(cv::Mat * frame);
    cv::Mat CallSingleHaar(cv::Mat input, cv::Mat img, cv::CascadeClassifier classif);
    cv::Mat ContourFinder(cv::Mat input, cv::Mat* img);
};


#endif //CSE573_PROJECT3_IMGPROCESSOR_H
