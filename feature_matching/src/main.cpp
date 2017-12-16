#include <iostream>
#include <vector>
#include <fstream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "../include/matrix.hpp"
#include "../include/utility.hpp"
#include "../include/gpu_matcher.hpp"

int main(int argc, char** argv)
{
    Timer timer;
    // Read image
    cv::Mat img_1, img_2;

    img_1 = cv::imread(argv[1]);
    if(img_1.empty() == true)
    {
        std::cout << "Image 1 could not be loaded!" << std::endl;
        return -1;
    }

    img_2 = cv::imread(argv[2]);
    if(img_2.empty() == true)
    {
        std::cout << "Image 2 could not be loaded!" << std::endl;
        return -1;
    }

    // Create SURF Detector
    int minHessian = 100;
    cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create();
    detector->setHessianThreshold(minHessian);

    // Detect keypoints using SURF descriptor
    std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
    cv::Mat descriptor_1, descriptor_2;

    detector->detectAndCompute(img_1, cv::Mat(), keypoints_1, descriptor_1);
    detector->detectAndCompute(img_2, cv::Mat(), keypoints_2, descriptor_2);

/*
    std::cout << "Num keypoints 1st img= " << keypoints_1.size() << std::endl;
    std::cout << "Num keypoints 2nd img= " << keypoints_2.size() << std::endl;
    std::cout << "Size of Descriptor = " << descriptor_1.size() << std::endl;
*/

/*
    // Write descriptor to file
    std::ofstream file;
    file.open("../features_SURF.txt");

    for(int i=0; i<descriptor_1.rows; i++)
    {
        for(int j=0; j<descriptor_1.cols; j++)
        {
            file << descriptor_1.at<float>(i,j);

            if(j != descriptor_1.cols-1)
                file << ",";
        }
        file << "\n";
    }

    file.close();
*/

/*
    // Print keypoints
    for(int i=0; i<keypoints_1.size(); i++)
    {
        std::cout << keypoints_1[i].pt << std::endl;
    }
*/

    startTimer(&timer);

    // Match feature points using GPUMatcher
    std::vector<cv::DMatch> matches;
    GPUMatcher matcher;
    matcher.match(descriptor_1, descriptor_2, matches);


    // Calculate max and min distance between keypoints
    double max_dist = 0, min_dist = 100;
    for(int i = 0; i < descriptor_1.rows; i++)
    {
        double dist = matches[i].distance;
        if(dist < min_dist)
            min_dist = dist;
        if(dist > max_dist)
            max_dist = dist;
    }

    // Get good matches
    std::vector<cv::DMatch> good_matches;

    for(int i = 0; i < descriptor_1.rows; i++)
    {
        if(matches[i].distance <= std::max(2*min_dist, 0.05))
            good_matches.push_back(matches[i]);
    }

    stopTimer(&timer);

    std::cout << "Match time = " << elapsedTime(timer) << " sec"<<std::endl;

    std::cout << "good_matches = " << good_matches.size() << std::endl;

    // Draw good matches
    //cv::Mat img_matches;
    //cv::drawMatches(img_1, keypoints_1, img_2, keypoints_2, good_matches,
    //                img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
    //                std::vector<char>(), 2);

    //cv::imshow("Matches", img_matches);
    //cv::waitKey(0);

    return 0;
}
