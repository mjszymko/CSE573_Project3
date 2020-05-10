//
// Created by michael on 5/9/20.
//

#ifndef CSE573_PROJECT3_MATH3D_H
#define CSE573_PROJECT3_MATH3D_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include <iostream>
#include <vector>

class Math3d {
private:

    std::vector<cv::Point> masterCorners;

    int maxContours = 5;

public:
    cv::Mat     GetMask(cv::Mat left, cv::Mat depth);
    bool        Calibrate(cv::Mat img, cv::Mat depth);
    cv::Mat     ApplyBorder(cv::Mat img);
    cv::Mat     GetWallMask(cv::Mat img);
};


#endif //CSE573_PROJECT3_MATH3D_H
