//--------------------------------------------------------------------------------------------------------------------//
// System Includes
#include <stdio.h>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// My Includes
#include "CameraFeed.h"
#include "ImgProcessor.h"
#include "Math3d.h"

//--------------------------------------------------------------------------------------------------------------------//
// Type Definitions

typedef enum
{
        eINIT = 0,
        eLOOP = 1,
} EState;

//--------------------------------------------------------------------------------------------------------------------//
// Local Variables
EState          state = eINIT;  // Tracks the state that the system is currently in
CameraFeed      cf;
ImgProcessor    ip;
Math3d          m3d;

cv::Mat         left;           // Left image (rectified)
cv::Mat         depth;          // Depth image (absolute depth)

static bool            gotDepth = false;
static bool            gotLeft  = false;

//--------------------------------------------------------------------------------------------------------------------//
bool Init();
void Loop();

//--------------------------------------------------------------------------------------------------------------------//

int main(int argc, char *argv[]) {

    cf.Init();
    ip.BackSubInit();
    ip.CreateTrackbar();

    while(true)
    {
        // Get Images
        gotDepth          = cf.GetDepth(&depth);
        gotLeft           = cf.GetLeftRectified(&left);

        //printf("gotDepth: %d and gotLeft %d\n", gotDepth, gotLeft);
//
//        printf("got here\n");

        if(!gotLeft) continue;

        imshow("left", left);

        switch(state)
        {
            case eINIT:
            {
                state = Init() ? eLOOP : eINIT;
            }
            case eLOOP:
            {
                Loop();
            }
            default:
            {

            }
        }

        char key = static_cast<char>(cv::waitKey(1));
        if (key == 27 || key == 'q' || key == 'Q') {  // ESC/Q
            break;
        }
    }

    cf.Terminate();
}

bool Init()
{
    cv::Mat img = left;
    cv::Mat depthLocal = depth;

    return m3d.Calibrate(img, depthLocal);
}


void Loop()
{
    cv::Mat img = left;

    img = m3d.ApplyBorder(img);



    cv::Mat img2 = left.clone();

    cv::Mat wallMask = m3d.GetWallMask(img2);

    imshow("wallmask", wallMask);

    cv::Mat fgMask = cv::Mat::zeros(img.size(), img.type());
//
    cv::Mat rando = left.clone();

    if(ip.BackSubApply("KNN", rando, &fgMask))
    {
        //cv::imshow("foreground", fgMask);
    }

    cv::Mat morph = left.clone();

    if(ip.MorphologyApply(fgMask, &morph))
    {
        //cv::imshow("morphology", morph);
    }

    cv::Mat fgInWall = cv::Mat::zeros(img.size(), img.type());
    cv::bitwise_and(morph, wallMask, fgInWall);

    cv::threshold(fgInWall, fgInWall,250, 255, 0);


    cv::imshow("fginWall", fgInWall);

    cv::Mat final = left.clone();

    cv::Mat contour = ip.ContourFinder(fgInWall, &final);

    cv::imshow("contour", contour);

    imshow("cimage", final);


}
