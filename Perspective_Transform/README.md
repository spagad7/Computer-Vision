# Perspective Transform

This is a program to apply perspective transform to an image to get a fronto-parallel view of the image. The steps to transform an image include:

1. calibrate camera
2. undistort image
3. calculate perspective transform
4. apply perspective transform to the input image

[//]:  # (Image Reference)
[image1]: dist_imgs/GOPR0034.jpg "Sample Calibration Image"
[image2]: test_img.jpg "Sample Test Image"
[image3]: img_trans.jpg "Sample Undistorted Image"

## Usage
This program takes 4 arguments:
1. path to directory containing chessboard images
2. path to distorted image
4. number of inner corners along x direction
5. number of inner corners along y direction

Example:
```
    python persp_trans.py dist_imgs test_img.jpg 8 6
```

## Sample Calibration Image
![alt text][image1]

## Sample Test Image
![alt text][image2]

## Output
The program displays the transformed image and writes the image as `img_trans.jpg` in the same directory.

![alt text][image3]
