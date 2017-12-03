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
    float x, y;
    // point of intersection of line with horizontal edge of image
    x = findX(pt1, pt2, img_edge.y);
    // point of intersection of line with verticle edge of image
    y = findY(pt1, pt2, img_edge.x);

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
Function to find x coordinate of a point for given y coordinate
Input:
    1. coordinates of first point on the line of type cv::Point2f
    2. coordinates of second point on the line of type cv::Point2f
    3. y coorindate of point whose x coordinate we want to find
Output:
    1. x coordinate of the point
*/
float findX(cv::Point2f pt1, cv::Point2f pt2, float y)
{
    float x, m, c;
    // get slope and y intersept of the line: y = mx + c
    m = (pt1.y - pt2.y) / (pt1.x - pt2.x);
    c = pt1.y - (m * pt1.x);
    x = (y - c) / m;

    return x;
}


/*
Function to find y coordinate of a point for given x coordinate
Input:
    1. coordinates of first point on the line of type cv::Point2f
    2. coordinates of second point on the line of type cv::Point2f
    3. x coorindate of point whose y coordinate we want to find
Output:
    1. y coordinate of the point
*/
float findY(cv::Point2f pt1, cv::Point2f pt2, float x)
{
    float y, m, c;
    // get slope and y intersept of the line: y = mx + c
    m = (pt1.y - pt2.y) / (pt1.x - pt2.x);
    c = pt1.y - (m * pt1.x);
    y = (m * x) + c;

    return y;
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


/*
Function to get coorindates of 5 rectangles in the image
Input:
    1. image of type cv::Mat
    2. corners of the calculated inner and outer rectangles
        vector of cv::Point2f
    3. unordered_map of vector of Point2fs in which corindates of 4
        corners of 5 rectangles will be stored
*/
void getRectCoords(cv::Mat& img, std::vector<cv::Point2f>& corners,
        std::unordered_map<std::string, std::vector<cv::Point2f>>& rectCoords)
{
    std::vector<cv::Point2f> inner_corners(corners.begin(), corners.begin()+4);
    std::vector<cv::Point2f> outer_corners(corners.begin()+4, corners.begin()+8);
    cv::Point2f vanish_pt = corners[8];


    // get coordinates of ceiling
    std::vector<cv::Point2f> ceiling;
    ceiling.push_back(outer_corners[0]);
    ceiling.push_back(outer_corners[1]);
    ceiling.push_back(inner_corners[1]);
    ceiling.push_back(inner_corners[0]);

    // get y coords of outer corners to same height
    if(ceiling[0].y < ceiling[1].y)
    {
        ceiling[0].x = findX(vanish_pt, ceiling[0], ceiling[1].y);
        ceiling[0].y = ceiling[1].y;
    }
    else
    {
        ceiling[1].x = findX(vanish_pt, ceiling[1], ceiling[0].y);
        ceiling[1].y = ceiling[0].y;
    }

    rectCoords["ceiling"] = ceiling;


    // get coordinates of floor
    std::vector<cv::Point2f> floorr;
    floorr.push_back(inner_corners[3]);
    floorr.push_back(inner_corners[2]);
    floorr.push_back(outer_corners[2]);
    floorr.push_back(outer_corners[3]);

    // get y coordinates of the outer corners to same height
    if(floorr[2].y > floorr[3].y)
    {
        floorr[2].x = findX(vanish_pt, floorr[2], floorr[3].y);
        floorr[2].y = floorr[3].y;
    }
    else
    {
        floorr[3].x = findX(vanish_pt, floorr[3], floorr[2].y);
        floorr[3].y = floorr[2].y;
    }

    rectCoords["floor"] = floorr;


    // get coordinates of left_wall
    std::vector<cv::Point2f> left;
    left.push_back(outer_corners[0]);
    left.push_back(inner_corners[0]);
    left.push_back(inner_corners[3]);
    left.push_back(outer_corners[3]);

    // get x coordinates of the outer corners to same width
    if(left[0].x < left[3].x)
    {
        left[0].y = findY(vanish_pt, left[0], left[3].x);
        left[0].x = left[3].x;
    }
    else
    {
        left[3].y = findY(vanish_pt, left[3], left[0].x);
        left[3].x = left[0].x;
    }

    rectCoords["left"] = left;


    // get coordinates of right_wall
    std::vector<cv::Point2f> right;
    right.push_back(inner_corners[1]);
    right.push_back(outer_corners[1]);
    right.push_back(outer_corners[2]);
    right.push_back(inner_corners[2]);

    // get x coorindates of the outer corners to same width
    if(right[1].x > right[2].x)
    {
        right[1].y = findY(vanish_pt, right[1], right[2].x);
        right[1].x = right[2].x;
    }
    else
    {
        right[2].y = findY(vanish_pt, right[2], right[1].x);
        right[2].x = right[1].x;
    }

    rectCoords["right"] = right;


    // get coordinates of the back_wall
    rectCoords["back"] = inner_corners;
}


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
                int& rows, int& cols)
{
    std::vector<cv::Point2f> tmp_vec;
    tmp_vec.push_back(rectCoords["ceiling"][0]);
    tmp_vec.push_back(rectCoords["floor"][3]);
    tmp_vec.push_back(rectCoords["left"][0]);
    int x_min = findMin(tmp_vec, 'x');

    tmp_vec.clear();
    tmp_vec.push_back(rectCoords["ceiling"][1]);
    tmp_vec.push_back(rectCoords["floor"][2]);
    tmp_vec.push_back(rectCoords["right"][2]);
    int x_max = findMax(tmp_vec, 'x');

    tmp_vec.clear();
    tmp_vec.push_back(rectCoords["left"][0]);
    tmp_vec.push_back(rectCoords["right"][1]);
    tmp_vec.push_back(rectCoords["ceiling"][0]);
    int y_min = findMin(tmp_vec, 'y');

    tmp_vec.clear();
    tmp_vec.push_back(rectCoords["left"][3]);
    tmp_vec.push_back(rectCoords["right"][2]);
    tmp_vec.push_back(rectCoords["floor"][2]);
    int y_max = findMax(tmp_vec, 'y');

    rows = y_max;
    cols = x_max;

    cubeFace.push_back(cv::Point2f(x_min, y_min));
    cubeFace.push_back(cv::Point2f(x_max, y_min));
    cubeFace.push_back(cv::Point2f(x_max, y_max));
    cubeFace.push_back(cv::Point2f(x_min, y_max));
}
