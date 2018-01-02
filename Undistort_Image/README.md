# Undistort Image

This is a program to calibrate camera and undistort a chessboard image using OpenCV library.

[//]:  # (Image Reference)
[image1]: dist_imgs/GOPR0034.jpg "Sample Calibration Image"
[image2]: test_img.jpg "Sample Test Image"
[image3]: img_undist.jpg "Sample Undistorted Image"

## Usage
This program takes 4 arguments:
1. path to directory containing chessboard images
2. path to distorted image
4. number of inner corners along x direction
5. number of inner corners along y direction

Example:
```
    python undistortImg.py dist_imgs test_img.jpg 8 6
```

## Sample Calibration Image
![alt text][image1]

## Sample Test Image
![alt text][image2]

## Output
The program displays the undistorted image and writes the image as `img_undist.jpg` in the same directory.

![alt text][image3]
