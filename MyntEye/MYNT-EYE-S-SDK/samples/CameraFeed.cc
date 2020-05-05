//
// Created by michael on 5/4/20.
//
#include "CameraFeed.h"

MYNTEYE_USE_NAMESPACE

bool CameraFeed::Init()
{
    int argc = 0;
    char *argv[1];

    api = API::Create(argc, argv);
    if (!api) return 1;

    // ************************************************************************************************************** //
    // Select which resolution you are using
    bool ok;
    auto &&request = api->SelectStreamRequest(&ok);
    if (!ok) return 1;
    api->ConfigStreamRequest(request);

    //api->SetDisparityComputingMethodType(DisparityComputingMethod::SGBM);
    api->EnableStreamData(Stream::DISPARITY_NORMALIZED);
    api->EnableStreamData(Stream::LEFT_RECTIFIED);

    api->Start(Source::VIDEO_STREAMING);


    api->SetOptionValue(Option::IR_CONTROL, 80);

    // ************************************************************************************************************** //

}

bool CameraFeed::GetLeft(cv::Mat *frame)
{
    api->WaitForStreams();

    auto &&left_data = api->GetStreamData(Stream::LEFT);

    if (!left_data.frame.empty()) {
        *frame = left_data.frame;
        return true;
    }
    return false;
}

bool CameraFeed::GetDepth(cv::Mat *frame)
{
    api->WaitForStreams();

    auto &&left_data = api->GetStreamData(Stream::DISPARITY_NORMALIZED);

    if (!left_data.frame.empty()) {
        *frame = left_data.frame;

        #ifdef WITH_OPENCV3
            cv::Mat colorMap;

            // ColormapTypes
            //   http://docs.opencv.org/master/d3/d50/group__imgproc__colormap.html#ga9a805d8262bcbe273f16be9ea2055a65
            cv::applyColorMap(left_data.frame, colorMap, cv::COLORMAP_JET);

            *frame = colorMap;

        #endif

        return true;
    }
    return false;
}

bool CameraFeed::GetLeftRectified(cv::Mat *frame)
{
    api->WaitForStreams();

    auto &&left_data = api->GetStreamData(Stream::LEFT_RECTIFIED);

    if (!left_data.frame.empty()) {
        *frame = left_data.frame;
        return true;
    }
    return false;
}

bool CameraFeed::Terminate()
{
    api->Stop(Source::VIDEO_STREAMING);
    return true;
}