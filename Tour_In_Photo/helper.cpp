#include "helper.hpp"

/*
Function to get corners of inner and outer rectangles in an images
Input:
    1. image of type cv::Mat
    2. vector of cv::Point
Output:
    void
*/
void getCorners(cv::Size img_size, std::vector<cv::Point2f>& corners)
{
    // Inner rectanlge coordinates in clockwise order
    // starting from top left corner
    corners.push_back(cv::Point2f(245.0,157.0));
    corners.push_back(cv::Point2f(2106.0, 157.0));
    corners.push_back(cv::Point2f(2106.0, 1424.0));
    corners.push_back(cv::Point2f(245.0, 1424.0));

    cv::Point2f vanish_pt(1132.5, 726.4620);

    // find points of intersection of image edges and the
    // lines connecting the vanishing point and the corners
    // of the inner rectangle
    corners.push_back(getIntersection(vanish_pt, corners[0],
                        cv::Point2f(0.0,0.0)));
    corners.push_back(getIntersection(vanish_pt, corners[1],
                        cv::Point2f(img_size.width, 0.0)));
    corners.push_back(getIntersection(vanish_pt, corners[2],
                        cv::Point2f(img_size.width, img_size.height)));
    corners.push_back(getIntersection(vanish_pt, corners[3],
                        cv::Point2f(0.0, img_size.height)));

    // insert vanishing point into the corners list
    corners.push_back(vanish_pt);
}


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
                            cv::Point2f img_edge)
{
    float x, y, m, c;
    // get slope and y intersept of the line
    // y = mx + c
    m = (pt1.y - pt2.y) / (pt1.x - pt2.x);
    c = pt1.y - (m * pt1.x);
    // point of intersection of line with horizontal edge of image
    x = (img_edge.y - c) / m;
    // point of intersection of line with verticle edge of image
    y = (m * img_edge.x) + c;

    cv::Point2f pt_intersect;
    // choose the farthest point of intersection on edges of the
    // image from the vanishing point as the correct point of
    // intersection
    (pow((pt1.x - img_edge.x), 2) + pow((pt1.y - y),2)) >
    (pow((pt1.x - x), 2) + pow((pt1.y - img_edge.y), 2)) ?
    x = img_edge.x : y = img_edge.y;

    pt_intersect.x = x;
    pt_intersect.y = y;

    return(pt_intersect);
}


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
                    std::vector<cv::Point2f>& corners)
{
    std::vector<cv::Point2f> outer_corners(corners.begin()+4,
                                corners.begin()+8);

    int margin_left = findMin(outer_corners, 'x');
    int margin_right = findMax(outer_corners, 'x')
                            - img.size().width;
    int margin_top = findMin(outer_corners, 'y');
    int margin_bottom = findMax(outer_corners, 'y')
                            - img.size().height;

    // Create bigger image to fit the outer rectangle
    cv::Mat img_big((img.size().height + margin_top + margin_bottom),
                    (img.size().width + margin_left + margin_right),
                    img.type());

    // Copy the source image into center of the bigger image
    img.copyTo(img_big(cv::Rect(margin_left, margin_top,
                        img.cols,img.rows)));

    img_big.copyTo(img_exp);

    // Adjust corner values
    for(int i=0; i<corners.size(); i++)
    {
        corners[i].x += margin_left;
        corners[i].y += margin_top;
    }
}


/*
Function to find minimum element in a vector of Point2fs
Input:
    1. vector of Point2f
    2. char idx ('x' or 'y')
Output:
    rounded and absolute minimum value
*/
int findMin(std::vector<cv::Point2f> outer_corners, char idx)
{
    float min = INT_MAX;

    for(int i=0; i<outer_corners.size(); i++)
    {
        if(idx == 'x' && outer_corners[i].x < min)
            min = outer_corners[i].x;
        else if(idx == 'y' && outer_corners[i].y < min)
            min = outer_corners[i].y;
    }

    return (int)round(abs(min));
}


/*
Function to find maximum element in a vector of Point2fs
Input:
    1. vector of Point2f
    2. char idx ('x' or 'y')
Output:
    rounded absolute maximum value
*/
int findMax(std::vector<cv::Point2f> outer_corners, char idx)
{
    float max = INT_MIN;

    for(int i=0; i<outer_corners.size(); i++)
    {
        if(idx == 'x' && outer_corners[i].x > max)
            max = outer_corners[i].x;
        else if(idx == 'y' && outer_corners[i].y > max)
            max = outer_corners[i].y;
    }

    return (int)round(fabs(max));
}
