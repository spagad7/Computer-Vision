#!/usr/bin/python3
import cv2
import argparse
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from helpers import *

if __name__ == '__main__':
    # Parse Arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('calib_path',
                        type=str,
                        help='path to calibration images')
    parser.add_argument('nx',
                        type=int,
                        help='number of inner corners along x direction')
    parser.add_argument('ny',
                        type=int,
                        help='number of inner corners along y direction')
    parser.add_argument('img_path',
                        type=str,
                        help='path to image file')
    args = parser.parse_args()

    mtx, dist = calibrateCamera(args.calib_path, args.nx, args.ny)
    # Read test image
    img = cv2.imread(args.img_path)
    # Undistort image
    img_undst = cv2.undistort(img, mtx, dist, None, mtx)
    # Apply color and gradient threshold to the image
    thresh = {#'sobel' : (20, 100),
              #'mag' : (20, 100),
              #'dir' : (0.7, 1.3),
              'hue_y' : (18, 30),
              'sat_y' : (20, 255),
              'light_y' : (60, 255),
              'hue_w' : (100, 110),
              'sat_w' : (10, 60),
              'light_w' : (130, 255),
              'hue_w2' : (0, 180),
              'sat_w2' : (0, 255),
              'light_w2' : (200, 255)}
    img_bin = convertToBinary(img_undst, thresh)

    # Plot the result
    f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
    f.tight_layout()
    ax1.imshow(img)
    ax1.set_title('Original Image', fontsize=50)
    ax2.imshow(img_bin, cmap='gray')
    ax2.set_title('Binary Image', fontsize=50)
    plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)
    plt.show()
