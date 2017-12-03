#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <vector>
#include <string>

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
Function to find x coordinate of a point for given y coordinate
Input:
    1. coordinates of first point on the line of type cv::Point2f
    2. coordinates of second point on the line of type cv::Point2f
    3. y coorindate of point whose x coordinate we want to find
Output:
    1. x coordinate of the point
*/
float findX(cv::Point2f pt1, cv::Point2f pt2, float y);


/*
Function to find y coordinate of a point for given x coordinate
Input:
    1. coordinates of first point on the line of type cv::Point2f
    2. coordinates of second point on the line of type cv::Point2f
    3. x coorindate of point whose y coordinate we want to find
Output:
    1. y coordinate of the point
*/
float findY(cv::Point2f pt1, cv::Point2f pt2, float y);


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


/*
Function to get coorindates of 5 rectangles in the image
Input:
    1. image of type cv::Mat
    2. corners of the calculated inner and outer rectangles
        vector of cv::Point2f
    3. vector of vector of Point2fs in which corindates of 4 corners of
        5 rectangles will be stored
*/
void getRectCoords(cv::Mat& img, std::vector<cv::Point2f>& corners,
        std::unordered_map<std::string, std::vector<cv::Point2f>>& rectCoords);


/*
Function to get coordinates of corners of face of cube
Input:
    1. unordered_map of vector of Point2fs in which contains coordinates of
        corners of all the faces of the cube
    2. vector of cv::Point2f to store the coodinates of cubeFace
*/
void getCubeFace(std::unordered_map<std::string,
                std::vector<cv::Point2f>>& rectCoords,
                std::vector<cv::Point2f>& cubeFace,
                int& rows, int& cols);
