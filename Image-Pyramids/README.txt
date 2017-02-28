Function prototype: 
[pyr] = getPyr(img, type, numLevels)

Function Description:
The function getPyr takes image, type and number of levels as arguments, and returns a cell array of images. The image can be color or gray scale, type can be ‘gauss’ or ‘laplace’ and number of levels indicate the number of levels in the image pyramid. 

In case of Gaussian pyramid, at each level, the image is smoothened by calling smoothenImg function and it is downsampled to half its size by calling downsampleImg. The image obtained at each level is stored in a cell array. 

In case of Laplacian pyramid, first, the Gaussian pyramid is generated. Next, at each level of generation of Laplacian pyramid, the image from the coarsest level of Gaussian pyramid is upsampled to twice its size by calling upsampleImg function, and then its smoothened by calling smoothenImg function. 

In the function smoothenImg, based on type of pyramid, I generate either XTX kernel for Gaussian pyramid and 4*XTX for Laplacian pyramid, and convolve the kernel with the image.

In the function downsampleImg, the input image is reduced to half its size by choosing every second pixel along the width and height of the image.

In the function upsampleImg, the input image is scaled to twice the size of the image by inserting zeros at every second row or column of the image.

Why the smoothing kernel is scaled by 4?
========================================
When an image is upsampled, we insert zeros in every second row and column of the image. Because of this insertion, the image size in quadrupled (increases by 4 times) and the pixels are spread across 4 times the area of original image. 


If we smoothen with H = XTX kernel, the image looks dull compared to the original image, because of the missing pixels. Hence to estimate the value of the missing pixels and to get the contrast and brightness of the original image we scale the  smoothing kernel 4 times. 
