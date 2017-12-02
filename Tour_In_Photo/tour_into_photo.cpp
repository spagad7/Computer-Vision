#include <iostream>
#include <opencv2/opencv.hpp>
#include "helper.hpp"

int main()
{
    cv::Mat img = cv::imread("../sample.jpg");
    cv::resize(img, img, cv::Size(), 0.5, 0.5);

    // Get cornes of rectangles
    std::vector<cv::Point2f> corners;
    getCorners(img.size(), corners);

    // expand the image to fit the outer rectangle
    cv::Mat img_exp;
    expandImage(img, img_exp, corners);

    // get faces of cube
    //getCubeFaces(img, )

    // Display image
    cv::imshow("Image", img_exp);
    cv::waitKey(0);
    return 0;
}
