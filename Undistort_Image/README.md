# Undistort Image

This is a sample program (in python) to calibrate camera and undistort an image using OpenCV library.

## Usage
This program takes 4 arguments:
1. path to directory containing chessboard images
2. path to distorted image
4. number of inner corners along x direction
5. number of inner corners along y direction

Example:
```
    python undistortImg.py ../Images test_img.jpg 8 6
```

# Output
The program writes `img_undist.jpg` in the same directory
