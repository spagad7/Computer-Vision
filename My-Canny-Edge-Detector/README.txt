mycanny.m
=========
This is my implementation of Canny edge detection algorithm. This matlab function also implements non-max suppression
and hysterisis thresholding to get finer edges. 

mygradient.m
============
Matlab function to calculate gradient magnitude and gradient direction of an input image. This function is used in mycanny function.



Function Prototype:
[Gm, Gd] = mygradient(img)

Description:
The function mygradient takes an image(color/grayscale) and returns gradient magnitude and gradient direction image. If the input image is colored, it is converted to gray scale and then to double format. Next, the gradient along the direction of x-axis is calculated by convolving the image with Prewitt kernel. Next, the gradient along the direction of y-axis is calculated by convolving the image with transpose of Prewitt kernel. The gradient magnitude is calculated by using the formula

 

Which has been implemented as 

Gm = sqrt(Gx.^2 + Gy.^2);

And the gradient direction is calculated by using 

 

Which has been implemented as

Gd = atan2(Gy, Gx);


Prewitt Kernel

[-1, 0, 1; 
 -1, 0, 1; 
 -1, 0, 1]
 
 
 
Function Prototype: 
edgeImg = mycanny(img)

Description:
The function takes a color/grayscale image as input and returns a binarized image with detected edges. The input image is converted to double and then to gray scale. Next, it’s convolved with Gaussian Kernel of size 3x3 and sigma value 1. After this, the gradient magnitude and direction is calculated using the mygradient function. The gradient magnitude and direction are then passed to Non max suppression to thin the edges in the gradient magnitude image. 
In non max suppression function, for each pixel, the two neighboring pixels (among 8 neighboring pixels) along the direction of gradient are found. The two neighboring pixels are chosen based on the angle of gradient, the angle threshold chosen in the function are shown in the diagram below. After finding the neighboring pixels, if the value of the current pixel is less than the value of either or the two neighboring pixels, then the pixel is suppressed. 

 

After non-max suppression, the suppressed image is passed to hysteresis thresholding function. In this function, the value of high and low threshold are set. And, all the pixels which have values greater than the threshold are retained and the pixels with the value lower than the low threshold are rejected. The pixels which fall in between high and low threshold, if any of the 8 neighboring pixels have value greater than the high threshold, then the value current pixel is set to a value greater than the high threshold. This will retain all the connected pixels. 

Choice of Parameters:
Number of neighboring pixels considered: 8
High Threshold = 	2.0 (in double)
Low Threshold = 	0.3 (in double)
Gaussian Kernel size = 	3x3
Sigma Value = 		1

A Gaussian kernel of larger size, say 5x5 will thicken the edges of gradient magnitude. And, sigma determines the width of the Gaussian, I am getting better results with sigma = 1.

