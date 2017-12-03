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
    std::unordered_map<std::string, std::vector<cv::Point2f>> rectCoords;
    getRectCoords(img_exp, corners, rectCoords);

/*
    std::unordered_map<std::string, std::vector<cv::Point2f>>::iterator it;
    for(it = rectCoords.begin(); it != rectCoords.end(); it++)
    {
        std::cout << it->first << std::endl;
        std::vector<cv::Point2f> coords = it->second;
        for(int i=0; i<coords.size(); i++)
            std::cout << coords[i] << std::endl;
        std::cout << "\n";
    }
*/

    // Get dimensions of fronto-parallel view of faces of cube
    std::vector<cv::Point2f> cubeFace;
    int rows, cols;
    getCubeFace(rectCoords, cubeFace, rows, cols);

    // get fronto parallel view images for each face of the cube
    // ceiling image
    cv::Mat H_ceil = findHomography(rectCoords["ceiling"], cubeFace);
    cv::Mat src_ceil = img_exp(cv::Rect(rectCoords["ceiling"][0],
                                            rectCoords["ceiling"][2]));
    //cv::Mat dst_ceil(rows, cols, src_ceil.type());
    cv::Mat dst_ceil;
    cv::warpPerspective(src_ceil, dst_ceil, H_ceil, dst_ceil.size());
    cv::imwrite("../ceil.jpg", dst_ceil);

    // floor image
    cv::Mat H_floor = findHomography(rectCoords["floor"], cubeFace);
    cv::Mat src_floor = img_exp(cv::Rect(rectCoords["floor"][0],
                                            rectCoords["floor"][2]));
    //cv::Mat dst_floor(rows, cols, src_floor.type());
    cv::Mat dst_floor;
    cv::warpPerspective(src_floor, dst_floor, H_floor, dst_floor.size());
    cv::imwrite("../floor.jpg", dst_floor);

    // left image
    cv::Mat H_left = findHomography(rectCoords["left"], cubeFace);
    cv::Mat src_left = img_exp(cv::Rect(rectCoords["left"][0],
                                            rectCoords["left"][2]));
    //cv::Mat dst_left(rows, cols, src_left.type());
    cv::Mat dst_left;
    cv::warpPerspective(src_left, dst_left, H_left, dst_left.size());
    cv::imwrite("../left.jpg", dst_left);

    // right image
    cv::Mat H_right = findHomography(rectCoords["right"], cubeFace);
    cv::Mat src_right = img_exp(cv::Rect(rectCoords["right"][0],
                                            rectCoords["right"][2]));
    //cv::Mat dst_right(rows, cols, src_right.type());
    cv::Mat dst_right;
    cv::warpPerspective(src_right, dst_right, H_right, dst_right.size());
    cv::imwrite("../right.jpg", dst_right);

    // back image
    cv::Mat H_back = findHomography(rectCoords["back"], cubeFace);
    cv::Mat src_back = img_exp(cv::Rect(rectCoords["back"][0],
                                            rectCoords["back"][2]));
    //cv::Mat dst_back(rows, cols, src_back.type());
    cv::Mat dst_back;
    cv::warpPerspective(src_back, dst_back, H_back, dst_back.size());
    cv::imwrite("../back.jpg", dst_back);

    return 0;
}
