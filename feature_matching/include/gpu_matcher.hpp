#ifndef GPUMATCHER
#define GPUMATCHER

#include <iostream>
#include <cmath>
#include "matrix.hpp"
#include "opencv2/core/core.hpp"

struct Match
{
    float distance;
    int idx1;
    int idx2;
};

class GPUMatcher
{
    public:
        GPUMatcher();
        void match(cv::Mat& desc1, cv::Mat& desc2,
                                std::vector<cv::DMatch>& matches);

    private:
        void AllocateDeviceMatrix(Matrix& M, int rows, int cols);
        void FreeDeviceMatrix(Matrix& M);
        void AllocateHostMatrix(Matrix& M, int rows, int cols);
        void AllocateHostMatchArray(Match* matches, int size);
        void AllocateDeviceMatchArray(Match* matches, int size);
        void FreeDeviceMatchArray(Match* matches);
        void FlattenMatrix(cv::Mat& M_Mat, Matrix& M);
        void CopyMatrixToDevice(Matrix& M_h, Matrix& M_d);
        void CopyMatrixToHost(Matrix M_d, Matrix M_h);
        void CopyMatchArrayToHost(Match* matches_d, Match* matches_h, int size);
        void CopyMatchArrayToDevice(Match* matches_h, Match* matches_d,int size);
        void ConvertMatchToDMatch(Match* matches_h,
                                    std::vector<cv::DMatch>& matches,
                                    int size);
};

#endif
