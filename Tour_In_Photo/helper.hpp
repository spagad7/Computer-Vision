#include <opencv2/opencv.hpp>
/*
Function to get corners of inner and outer rectangles in an images
Input:
    1. image of type cv::Mat
    2. vector of cv::Point2f
Output:
    void
*/
void getCorners(cv::Size img_size, std::vector<cv::Point2f>& corners);


/*
Function to get point of intersection of line with image
Input:
    1. 1st point of type Point2f on the line
    2. 2nd point of type Point 2f on the line
    3. image edges of type Point2f
Output:
    1. point of intersection of type Point2f
*/
cv::Point2f getIntersection(cv::Point2f pt1, cv::Point2f pt2,
                            cv::Point2f img_edge);


/*
Function to expand image to fit outer rectangle
Input:
    1. source image
    2. destination image
    3. corners of the calculated inner and outer rectangles
        vector of cv::Point2f
Output:
    void
*/
void expandImage(cv::Mat& img, cv::Mat& img_exp,
                    std::vector<cv::Point2f>& corners);


/*
Function to find minimum element in a vector of Point2fs
Input:
    1. vector of Point2f
    2. char idx ('x' or 'y')
Output:
    rounded absolute minimum value
*/
int findMin(std::vector<cv::Point2f> outer_corners, char idx);


/*
Function to find maximum element in a vector of Point2fs
Input:
    1. vector of Point2f
    2. char idx ('x' or 'y')
Output:
    rounded absolute maximum value
*/
int findMax(std::vector<cv::Point2f> outer_corners, char idx);
