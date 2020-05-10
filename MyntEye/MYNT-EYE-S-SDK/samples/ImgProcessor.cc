//
// Created by michael on 5/4/20.
//

#include "ImgProcessor.h"

#define YMAX

bool ImgProcessor::CreateTrackbar()
{
    cv::namedWindow(tbn);
    //cv::namedWindow("win2");
    cv::namedWindow("win3");

    morphKernelSz = 13;
    cv::createTrackbar( "Morphology Kernel Size", tbn, &morphKernelSz, 25);

    morphTypes = 4;
    cv::createTrackbar( "Morphology Type", tbn, &morphTypes, 7);

    learningRate = 0;
    cv::createTrackbar( "learning rate", tbn, &learningRate, 100);

    retrievalMode = 3;
    cv::createTrackbar( "Retrieval Mode", "win3", &retrievalMode, 4);

    contourApproxMode = 1;
    cv::createTrackbar( "Contour Method", "win3", &contourApproxMode, 4);

    cannyThreshLow = 242;
    cv::createTrackbar( "Canny Threshold Lo", "win3", &cannyThreshLow, 255);

    cannyThreshHigh = 240;
    cv::createTrackbar( "Canny Threshold Hi", "win3", &cannyThreshHigh, 255);

    apertureSize = 3;
    cv::createTrackbar( "Canny aperture", "win3", &apertureSize, 7);

    maxContours = 5;
    cv::createTrackbar( "CMax Contours", "win3", &maxContours, 50);

    erosionMask = 3;
    cv::createTrackbar( "Erosion Mask", "win3", &erosionMask, 20);



}

bool ImgProcessor::BackSubInit()
{
    // default is history = 500, varThreshold = 16, detectShadows = 1
    pBackSubMOG2 = cv::createBackgroundSubtractorMOG2(100, 5, 0);

    pBackSubKNN = cv::createBackgroundSubtractorKNN(300, 600, false);

    return true;
}

bool ImgProcessor::BackSubApply(std::string selection, cv::Mat frame, cv::Mat * fgmask)
{
    if(selection == "MOG2")
    {
        float tmpLearnRate = learningRate/100;
        pBackSubMOG2->apply(frame,*fgmask, tmpLearnRate);
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
    cv::Mat element = getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( morphKernelSz,morphKernelSz), cv::Point( morphKernelSz/2, morphKernelSz/2 ) );

    cv::Mat erosionK = getStructuringElement( cv::MORPH_ELLIPSE, cv::Size( erosionMask,erosionMask), cv::Point( erosionMask/2, erosionMask/2 ) );

    morphologyEx( frame, *mask, cv::MORPH_ERODE, erosionK );

    // cv::MORPH_OPEN gives good performance
    morphologyEx( *mask, *mask, morphTypes, element );



    // extra stuff might break it

    return true;
}

bool ImgProcessor::HaarClassifierInit()
{
    bool rollup = closed_palm_cascade.load("/home/michael/VirtualWall/CSE573_Project3/MyntEye/OpenCV/opencv/data/haarcascades/closed_palm.xml")
    | fist_cascade.load("/home/michael/VirtualWall/CSE573_Project3/MyntEye/OpenCV/opencv/data/haarcascades/fist.xml")
    | closed_frontal_palm_cascade.load("/home/michael/VirtualWall/CSE573_Project3/MyntEye/OpenCV/opencv/data/haarcascades/closed_frontal_palm.xml")
    | aGest_cascade.load("/home/michael/VirtualWall/CSE573_Project3/MyntEye/OpenCV/opencv/data/haarcascades/aGest.xml")
    | palm_cascade.load("/home/michael/VirtualWall/CSE573_Project3/MyntEye/OpenCV/opencv/data/haarcascades/palm.xml")
    | open_palm_cascade.load("/home/michael/VirtualWall/CSE573_Project3/MyntEye/OpenCV/opencv/data/haarcascades/open_palm.xml");

    printf("Loaded hand cascade: %d\n", rollup);

    return rollup;
}


// Detect Hand
bool ImgProcessor::DetectHand(cv::Mat * frame)
{
    cv::Mat tmp = *frame;
    cv::Mat img = *frame;

    img = CallSingleHaar(tmp, img, palm_cascade);
    img = CallSingleHaar(tmp, img, fist_cascade);
    img = CallSingleHaar(tmp, img, closed_frontal_palm_cascade);
    img = CallSingleHaar(tmp, img, aGest_cascade);
    img = CallSingleHaar(tmp, img, open_palm_cascade);




    *frame = img;

    return true;

}

cv::Mat ImgProcessor::CallSingleHaar(cv::Mat input, cv::Mat img, cv::CascadeClassifier classif)
{
    std::vector<cv::Rect> faces;

    classif.detectMultiScale( input, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30), cv::Size(480, 480) );

    for( size_t i = 0; i < faces.size(); i++ ) {
        cv::Point center(faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height * 0.5);
        ellipse(img, center, cv::Size(faces[i].width * 0.5, faces[i].height * 0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
    }

    return img;
}

struct lessThan
{
    inline bool operator() (std::vector<cv::Point> c1, std::vector<cv::Point> c2)
    {
        return (cv::contourArea(c1) > cv::contourArea(c2));
    }
};

cv::Mat ImgProcessor::ContourFinder(cv::Mat input, cv::Mat* img)
{
    cv::Mat frame = input;

    cv::RNG rng(12345);

    cv::Mat canny_output;

//    InputArray image, OutputArray edges, double threshold1, double threshold2,
//    int apertureSize = 3, bool L2gradient = false );

    cv::Canny( frame, canny_output, cannyThreshLow, cannyThreshHigh, apertureSize);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    findContours( canny_output, contours, hierarchy, retrievalMode, contourApproxMode );
    cv::Mat drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );

    std::sort (contours.begin(), contours.end(), lessThan());

    int contourCnt = contours.size();

    if(contourCnt == 0) return drawing;
    if(contours[0].size() == 0) return drawing;

    int contourRange = std::min(contourCnt, maxContours);


//    void cv::drawContours( InputOutputArray _image, InputArrayOfArrays _contours,
//                           int contourIdx, const Scalar& color, int thickness,
//                           int lineType, InputArray _hierarchy,
//                           int maxLevel, Point offset )

    std::vector<std::vector<cv::Point> > hull( contourRange );
    for( size_t i = 0; i < contourRange; i++ )
    {
        convexHull( contours[i], hull[i] );
    }

    /// Get the moments
    std::vector<cv::Moments> mu(contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { mu[i] = cv::moments( contours[i], false ); }

    ///  Get the mass centers:
    std::vector<cv::Point2f> mc( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

#ifdef  YMAX
    int contourIdx = 0;
    int yMin = contours[0][0].y;

    for (int j = 0; j < contours.size(); j++) {
        for(int k = 0; k < contours[j].size(); k++)
        {
            if(contours[j][k].y < yMin)
            {
                yMin = contours[j][k].y;
                contourIdx = j;
            }

        }
    }
#endif

#ifdef XMAX
    int contourIdx = 0;
    int xMaxVal = contours[0][0].x;

    for (int j = 0; j < contours.size(); j++) {
        for(int k = 0; k < contours[j].size(); k++)
        {
            if(contours[j][k].y > xMaxVal)
            {
                xMaxVal = contours[j][k].x;
                contourIdx = j;
            }

        }
    }
#endif
//    for( size_t i = 0; i< contourRange; i++ )
//    {
//        cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
//        cv::drawContours( drawing, contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0 );
//        cv::drawContours( drawing, hull, (int)i, color );
//    }

    cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
    cv::drawContours( drawing, contours, contourIdx, color, 2, cv::LINE_8, hierarchy, 0 );
//    cv::drawContours( drawing, hull, maxContourIdx, color );

    cv::Point fingertip;
    fingertip.x = 0;
    fingertip.y = 0;
    double xMax = 0;
//    bool firstTime = true;

    for (int l = 0; l < contours[contourIdx].size(); l++)
    {

        if(contours[contourIdx][l].x > xMax)
        {
            xMax = contours[contourIdx][l].x;
            fingertip.x = contours[contourIdx][l].x;
            fingertip.y = contours[contourIdx][l].y;
        }
    }

    cv::circle(*img, fingertip, 5, cv::Scalar(255), 5);

    return drawing;
}
