#ifndef KERNELS
#define KERNELS

#include "../include/matrix.hpp"
#include <climits>


// Naive implementation of matrix multiplication kernel
__global__ void matmult_kernel_v1(Matrix M, Matrix N, Matrix P)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    float sum = 0;

    if(row < P.rows && col < P.cols)
    {
        for(int i=0; i<M.cols; i++)
            sum += M.elements[row*M.cols + i] * N.elements[i*N.cols + col];

        P.elements[row*P.cols + col] = sum;
    }
}



// Naive implementation of finding minimum element in a row
__global__ void find_min(Matrix M, Match* matches)
{
    int tIdx = blockIdx.x * blockDim.x + threadIdx.x;
    float min = INT_MAX;
    int minIdx;

    if(tIdx < M.rows)
    {
        for(int i=0; i<M.cols; i++)
        {
            if(M.elements[tIdx*M.cols + i] < min)
            {
                min = M.elements[tIdx*M.cols + i];
                minIdx = i;
            }
        }

        matches[tIdx].distance = min;
        matches[tIdx].idx1 = tIdx;
        matches[tIdx].idx2 = minIdx;
    }
}


__global__ void dummy()
{
    int tIdx = blockIdx.x * blockDim.x + threadIdx.x;
    int tmp = 0;
}

#endif
