//
// Created by michael on 5/3/20.
//
#include <stdio.h>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "CameraFeed.h"
#include "ImgProcessor.h"

int main(int argc, char *argv[]) {

    CameraFeed cf;
    ImgProcessor ip;

    cf.Init();
    ip.BackSubInit();

    while(true)
    {
        cv::Mat left;
        cv::Mat depth;
        cv::Mat leftRec;

        bool gotLeft    = cf.GetLeft(&left);
        bool gotDepth   = cf.GetDepth(&depth);
        bool gotLeftRec = cf.GetLeftRectified(&leftRec);

        if(gotLeft)
        {
            cv::imshow("left", left);
        }

        if(gotLeftRec)
        {
            cv::imshow("left_rec", leftRec);
        }


        if(gotDepth)
        {
            cv::imshow("depth", depth);
        }

        cv::Mat fgMask;

        if(ip.BackSubApply("KNN", left, &fgMask))
        {
            cv::imshow("foreground", fgMask);
        }

        cv::Mat morph;

        if(ip.MorphologyApply(fgMask, &morph))
        {
            cv::imshow("morphology", morph);
        }

        char key = static_cast<char>(cv::waitKey(1));
        if (key == 27 || key == 'q' || key == 'Q') {  // ESC/Q
            break;
        }
    }

    cf.Terminate();
}
