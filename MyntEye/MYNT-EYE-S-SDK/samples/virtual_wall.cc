//
// Created by michael on 5/3/20.
//
#include <stdio.h>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "mynteye/api/api.h"

MYNTEYE_USE_NAMESPACE

int main(int argc, char *argv[]) {

    // Create the Mynt Eye API Object
    auto &&api = API::Create(argc, argv);
    if (!api) return 1;

    // ************************************************************************************************************** //
    // Select which resolution you are using
    bool ok;
    auto &&request = api->SelectStreamRequest(&ok);
    if (!ok) return 1;
    api->ConfigStreamRequest(request);


    double fps;
    double t = 0.01;
    std::cout << "depth fps:" << std::endl;

    api->EnableStreamData(Stream::DEPTH);
    api->EnableStreamData(Stream::DISPARITY_NORMALIZED);

    api->Start(Source::VIDEO_STREAMING);

    cv::namedWindow("frame");
    cv::namedWindow("depth_real");
    cv::namedWindow("depth_normalized");
    cv::namedWindow("left");

    // ************************************************************************************************************** //
    // Set up video writers for recording
    api->WaitForStreams();
    auto &&sample = api->GetStreamData(Stream::LEFT);

    while (sample.frame.empty())
    {
        api->WaitForStreams();
        sample = api->GetStreamData(Stream::LEFT);
    }

    cv::VideoWriter videoLeft(  R"(/home/michael/VirtualWall/CSE573_Project3/video/left.avi)",
                                CV_FOURCC('X', 'V', 'I', 'D'),
                                25,
                                cv::Size(sample.frame.cols, sample.frame.rows),
                                0
                              );

    api->WaitForStreams();
    auto &&sample2 = api->GetStreamData(Stream::DISPARITY_NORMALIZED);

    while (sample2.frame.empty())
    {
        api->WaitForStreams();
        sample2 = api->GetStreamData(Stream::DISPARITY_NORMALIZED);
    }

    cv::VideoWriter videoDepth (  R"(/home/michael/VirtualWall/CSE573_Project3/video/depth.avi)",
                                 CV_FOURCC('X', 'V', 'I', 'D'),
                                 25,
                                 cv::Size(sample2.frame.cols, sample2.frame.rows),
                                 0
                               );

    printf("left video writer is opened: %d\n", videoLeft.isOpened());

    // ************************************************************************************************************** //

    while (true) {
        api->WaitForStreams();

        auto &&left_data = api->GetStreamData(Stream::LEFT);
        auto &&right_data = api->GetStreamData(Stream::RIGHT);

        if (!left_data.frame.empty() && !right_data.frame.empty()) {
            cv::Mat img = left_data.frame;
            //cv::hconcat(left_data.frame, right_data.frame, img);
            cv::imshow("left", img);

            cv::Mat img2 = right_data.frame;
            cv::imshow("right", img2);

            // Rows = 480 and Columns = 752
//            printf("left dimensions are: rows %d and columns %d\n", img.rows, img.cols);
//            printf("right dimensions are: rows %d and columns %d\n", img2.rows, img2.cols);

            videoLeft.write(img);
        }

        // this code is for real depth data
        auto &&depth_data = api->GetStreamData(Stream::DISPARITY_NORMALIZED);
        if (!depth_data.frame.empty()) {
            double t_c = cv::getTickCount() / cv::getTickFrequency();
            fps = 1.0/(t_c - t);
            //printf("\b\b\b\b\b\b\b\b\b%.2f", fps);
            t = t_c;
            cv::imshow("depth_real", depth_data.frame);  // CV_16UC1

            //printf("depth dimensions are: rows %d and columns %d\n", depth_data.frame.rows, depth_data.frame.cols);

            videoDepth.write(depth_data.frame);
        }

        // this code is for normalized depth data
//        auto &&disp_norm_data = api->GetStreamData(Stream::DISPARITY_NORMALIZED);
//        if (!disp_norm_data.frame.empty()) {
//            cv::imshow("depth_normalized", disp_norm_data.frame);  // CV_8UC1
//        }

        char key = static_cast<char>(cv::waitKey(1));
        if (key == 27 || key == 'q' || key == 'Q') {  // ESC/Q
            break;
        }
    }

    api->Stop(Source::VIDEO_STREAMING);
    return 0;
}
