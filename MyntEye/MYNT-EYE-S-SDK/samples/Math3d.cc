//
// Created by michael on 5/9/20.
//

#include "Math3d.h"
//#define UNUSED_CODE

struct lessThan
{
    inline bool operator() (std::vector<cv::Point> c1, std::vector<cv::Point> c2)
    {
        return (cv::contourArea(c1) > cv::contourArea(c2));
    }
};

cv::Mat     Math3d::GetMask(cv::Mat left, cv::Mat depth)
{

}

bool        Math3d::Calibrate(cv::Mat img, cv::Mat depth)
{
    std::vector<cv::Point> tmpCorners;
    std::vector<cv::Point> corners;

    //----------------------------------------------------------------------------------------------------------------//
    cv::Mat binary;     // thresholded image

    cv::threshold(img, binary,120, 255, 0);

    binary = cv::Scalar::all(255) - binary;

    //----------------------------------------------------------------------------------------------------------------//
    // Identify the virtual wall in a sea of contours


    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> lgContours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours( binary, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    int contourCnt = contours.size();
    int contourRange = std::min(contourCnt, maxContours);

    // Sort the contours
    std::sort (contours.begin(), contours.end(), lessThan());

    bool foundWall = false;

    for (int i = 0; i < contourCnt; ++i)
    {
        //lgContours.push_back(contours.at(i));
        double area = cv::contourArea(contours[i]);

        if(35000 < area && area < 50000 )
        {
            double perim = cv::arcLength(contours[i],true);
            std::vector<cv::Point> sides;
            cv::approxPolyDP(contours[i], sides, 0.05*perim, true);
            if(sides.size() == 4)
            {
                tmpCorners = sides;
                foundWall = true;
                break;
            }
        }
    }

    if(foundWall == false) return false;

    //----------------------------------------------------------------------------------------------------------------//
    // Find top left corner and order the corners counterclockwise starting from the top left

    std::vector<double> coordSums;

    for (int j = 0; j < tmpCorners.size(); ++j)
    {
        double sum = tmpCorners[j].x + tmpCorners[j].y;
        coordSums.push_back(sum);
    }

    int minIdx = std::distance(coordSums.begin(),std::min_element(coordSums.begin(), coordSums.end()));

    printf("min index is %d\n", minIdx);

#ifndef UNUSED_CODE

    corners.push_back(tmpCorners[minIdx]);

    for (int k = minIdx; k < tmpCorners.size(); ++k)
    {
        if(k == minIdx) continue;
        corners.push_back(tmpCorners[k]);
    }

    for (int k = 0; k < minIdx; ++k)
    {
        if(k == minIdx) continue;
        corners.push_back(tmpCorners[k]);
    }

    //----------------------------------------------------------------------------------------------------------------//
    // Drawing

//    cv::RNG rng(12345);
//
//    for (int l = 0; l < corners.size(); ++l)
//    {
//        cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
//        cv::circle(img, corners[l], 15, color, 5);
//    }
//
//    cv::Scalar tcolor = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
//
//    std::vector<std::vector<cv::Point>> oneCorner;
//    oneCorner.push_back(corners);
//
//    cv::drawContours( img, oneCorner, 0, tcolor, 5 );

    //2.circle(img, center, radius, color, thickness=1, lineType=8, shift=0)

#endif

    masterCorners = corners;

    return true;

//    cv::imshow("binary", binary);
//    cv::imshow("c image", img);
}

cv::Mat Math3d::ApplyBorder(cv::Mat img)
{
    cv::RNG rng(12345);
    cv::Scalar tcolor = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );

    std::vector<std::vector<cv::Point>> oneCorner;
    oneCorner.push_back(masterCorners);

    cv::drawContours( img, oneCorner, 0, tcolor, 5 );

    return img;
}

cv::Mat Math3d::GetWallMask(cv::Mat img)
{
    cv::Mat tmp = cv::Mat::zeros(img.size(), img.type());

//    std::vector<cv::Point> *  ptr = &masterCorners;
//    cv::Point ** dptr = &ptr;
//    cv::Scalar tcolor = cv::Scalar( 255, 255, 255 );

    //printf("sixze of tmp %d %d\n", tmp.rows, tmp.cols);

    std::vector<std::vector<cv::Point>> oneCorner;
    oneCorner.push_back(masterCorners);

    cv::fillPoly(tmp, oneCorner, cv::Scalar(255));

//    void cv::fillPoly(InputOutputArray _img, InputArrayOfArrays pts,
//                      const Scalar& color, int lineType, int shift, Point offset)

    //imshow("tmp", tmp);

    //cv2.fillPoly(img, pts =[contours], color=(255,255,255))

    return tmp;
}
