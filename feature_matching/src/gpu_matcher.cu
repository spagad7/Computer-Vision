#include "../include/gpu_matcher.hpp"
#include "kernels.cu"


// Constructor for GPU Matcher
GPUMatcher::GPUMatcher(){}


// Function to perform feature matching on GPU
void GPUMatcher::match(cv::Mat& desc1, cv::Mat& desc2,
                        std::vector<cv::DMatch>& matches)
{
    Matrix desc1_h, desc2_h, product_mat_h;
    Matrix desc1_d, desc2_d, product_mat_d;

    // Transpose desc1 matrix
    cv::Mat desc2_trans;
    cv::transpose(desc2, desc2_trans);

    // Allocate matrices on GPU
    AllocateDeviceMatrix(desc1_d, desc1.rows, desc1.cols);
    AllocateDeviceMatrix(desc2_d, desc2_trans.rows, desc2_trans.cols);
    AllocateDeviceMatrix(product_mat_d, desc1.rows, desc2_trans.cols);

    // Flatten matrices
    FlattenMatrix(desc1, desc1_h);
    FlattenMatrix(desc2_trans, desc2_h);

    // Allocate Matrix on Host
    //AllocateHostMatrix(product_mat_h, desc1.rows, desc2_trans.cols);

    // Copy flattened matrices to device memory
    CopyMatrixToDevice(desc1_h, desc1_d);
    CopyMatrixToDevice(desc2_h, desc2_d);

/*

// TODO: Implement dynamic block size based on product_mat_d size

    // Configure matrix multiplication kernel
    cudaDeviceProp prop;
    int deviceId = 0;
    cudaError_t ret_val = cudaGetDeviceProperties(&prop, deviceId);
    if(ret_val != cudaSuccess)
    {
        std::cout << "Error getting cuda device property!" << std::endl;
        exit(-1);
    }

    dim3 dimBlock, dimGrid;
    if(product_mat_d.rows*product_mat_d.cols < prop.maxThreadsPerBlock)
    {

    }
*/

    dim3 dimBlock, dimGrid;
    dimBlock.x = 16, dimBlock.y = 16, dimBlock.z=1;
    dimGrid.x = (int)ceil((float)product_mat_d.cols/dimBlock.x);
    dimGrid.y = (int)ceil((float)product_mat_d.rows/dimBlock.y);
    dimGrid.z = 1;

    // Launch matrix multiplication kernel
    matmult_kernel_v1<<<dimGrid, dimBlock>>>(desc1_d, desc2_d, product_mat_d);

    cudaDeviceSynchronize();
    // Copy matrix to host for testing
    //CopyMatrixToHost(product_mat_d, product_mat_h);

    // Allocate memory for array of match objects in host and device
    Match *matches_h, *matches_d;
    //AllocateHostMatchArray(matches_h, product_mat_d.rows);
    //AllocateDeviceMatchArray(matches_d, product_mat_d.rows);

    matches_h = (Match *)malloc(product_mat_d.rows * sizeof(Match));
    if(matches_h == NULL)
    {
        std::cout << "Error allocating host memory!" << std::endl;
        exit(-1);
    }

    cudaError_t ret_val = cudaMalloc((void**)&matches_d, product_mat_d.rows * sizeof(Match));
    if(ret_val != cudaSuccess)
    {
        std::cout << "Error allocating memory on device!" << std::endl;
        exit(-1);
    }

    // Configure find_min kernel
    dimBlock.x = 256;
    dimBlock.y = 1;
    dimBlock.z = 1;
    dimGrid.x = (int)ceil((float)product_mat_d.rows/dimBlock.x);
    dimGrid.y = 1;
    dimGrid.z = 1;

    // Launch find_min kernel
    find_min<<<dimGrid, dimBlock>>>(product_mat_d, matches_d);
    //dummy<<<dimGrid, dimBlock>>>();
    cudaDeviceSynchronize();

    // Copy Match array from device
    CopyMatchArrayToHost(matches_d, matches_h, product_mat_d.rows);

    // Convert Match array to vector of DMatch objects
    ConvertMatchToDMatch(matches_h, matches, product_mat_d.rows);
}


// Function to allocate matrix on device
void GPUMatcher::AllocateDeviceMatrix(Matrix& M, int rows, int cols)
{
    M.rows = rows;
    M.cols = cols;
    int size = rows * cols * sizeof(float);
    cudaError_t ret_val = cudaMalloc((void**)&M.elements, size);
    if(ret_val != cudaSuccess)
    {
        std::cout << "Error allocating memory on device!" << std::endl;
        exit(-1);
    }
}


// Function to free Matrix in device memory
void GPUMatcher::FreeDeviceMatrix(Matrix& M)
{
    cudaError_t ret_val = cudaFree(M.elements);
    if(ret_val != cudaSuccess)
    {
        std::cout << "Unable to free allocated device memory!" << std::endl;
    }
    M.elements = NULL;
}


// Function to allocate matrix in host memory
void GPUMatcher::AllocateHostMatrix(Matrix& M, int rows, int cols)
{
    M.rows = rows;
    M.cols = cols;
    int size = rows * cols * sizeof(float);

    M.elements = (float *)malloc(size);
    if(M.elements == NULL)
    {
        std::cout << "Error allocating host memory!" << std::endl;
        exit(-1);
    }

}


// Function to allocate array of Match objects in host memory
void GPUMatcher::AllocateHostMatchArray(Match* matches, int size)
{
    matches = (Match *)malloc(size * sizeof(Match));
    if(matches == NULL)
    {
        std::cout << "Error allocating host memory!" << std::endl;
        exit(-1);
    }
}


// Function to allocate array of Match objects in device memory
void GPUMatcher::AllocateDeviceMatchArray(Match* matches, int size)
{
    cudaError_t ret_val = cudaMalloc((void**)&matches, size * sizeof(Match));
    if(ret_val != cudaSuccess)
    {
        std::cout << "Error allocating memory on device!" << std::endl;
        exit(-1);
    }
}


// Function to free device Match array
void GPUMatcher::FreeDeviceMatchArray(Match* matches)
{
    cudaError_t ret_val = cudaFree(matches);
    if(ret_val != cudaSuccess)
    {
        std::cout << "Unable to free allocated device memory!" << std::endl;
    }
    matches = NULL;
}


// Function to flatten 2D matrix
void GPUMatcher::FlattenMatrix(cv::Mat& M_Mat, Matrix& M)
{
    M.rows = M_Mat.rows;
    M.cols = M_Mat.cols;
    M.elements = (float *)malloc(M_Mat.rows * M_Mat.cols * sizeof(float));

    for(int i=0; i<M_Mat.rows; i++)
    {
        for(int j=0; j<M_Mat.cols; j++)
        {
            M.elements[i * M_Mat.cols + j] = M_Mat.at<float>(i,j);
        }
    }
}


// Function to copy matrix from host to device
void GPUMatcher::CopyMatrixToDevice(Matrix& M_h, Matrix& M_d)
{
    int size = M_h.rows * M_h.cols * sizeof(float);
    cudaError_t ret_val = cudaMemcpy(M_d.elements, M_h.elements, size,
                                    cudaMemcpyHostToDevice);
    if(ret_val != cudaSuccess)
    {
        std::cout << "Unable to copy data to device memory" << std::endl;
        exit(-1);
    }
}


// Function to copy matrix from device to host
void GPUMatcher::CopyMatrixToHost(Matrix M_d, Matrix M_h)
{
    M_h.rows = M_d.rows;
    M_h.cols = M_d.cols;
    int size = M_d.rows * M_d.cols * sizeof(float);
    cudaError_t ret_val = cudaMemcpy(M_h.elements, M_d.elements, size,
                                    cudaMemcpyDeviceToHost);
    if(ret_val != cudaSuccess)
    {
        std::cout << "Unable to copy data fom device to host" << std::endl;
        exit(-1);
    }
}


// Function to copy array of Match objects from device to host memory
void GPUMatcher::CopyMatchArrayToHost(Match* matches_d, Match* matches_h,
                                        int size)
{
    cudaError_t ret_val = cudaMemcpy(matches_h, matches_d, size*sizeof(Match),
                                        cudaMemcpyDeviceToHost);
    if(ret_val != cudaSuccess)
    {
        std::cout << "Error copying data from device to host!" << std::endl;
        exit(-1);
    }
}


// Function to copy array of Match objects from device to host memory
void GPUMatcher::CopyMatchArrayToDevice(Match* matches_h, Match* matches_d,
                                        int size)
{
    cudaError_t ret_val = cudaMemcpy(matches_d, matches_h, size*sizeof(Match),
                                        cudaMemcpyHostToDevice);
    if(ret_val != cudaSuccess)
    {
        std::cout << "Error copying data from host to device!" << std::endl;
        exit(-1);
    }
}


// Function to conver array of Match objects to vector of cv::DMatch objects
void GPUMatcher::ConvertMatchToDMatch(Match* matches_h,
                                        std::vector<cv::DMatch>& matches,
                                        int size)
{
    for(int i=0; i<size; i++)
    {
        matches.push_back(cv::DMatch(matches_h[i].idx1, matches_h[i].idx2,
                                    0, matches_h[i].distance));
    }
}
