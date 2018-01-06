#!/usr/bin/python3
import cv2
import argparse
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from moviepy.editor import VideoFileClip
from calibration import *
from threshold import *

if __name__ == '__main__':
    # Parse Arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('-c',
                        type=str,
                        help='path to calibration file')
    parser.add_argument('-p',
                        type=str,
                        help='path to calibration images')
    parser.add_argument('-nx',
                        type=int,
                        help='number of inner corners along x direction')
    parser.add_argument('-ny',
                        type=int,
                        help='number of inner corners along y direction')
    parser.add_argument('-i',
                        type=str,
                        help='path to image file')
    parser.add_argument('-v',
                        type=str,
                        help='path to video file')
    args = parser.parse_args()

    # Error checking
    if(args.c == None and args.p == None):
        print("Insufficient arguments: either calib file or path to calib\
                images must be speicified!")
        quit()
    elif(args.p != None and (args.c == None or args.nx == None
        or args.ny == None)):
        print("Insufficient arguments: must specify calib file name, nx and ny")
        quit()

    if(args.i == None and args.v == None):
        print("Insufficient arguments: either image or video file must be\
                specified!")
        quit()

    # Calibrate Camera
    if args.p != None:
        mtx, dist = calibrateCamera(args.p, args.nx, args.ny)
        saveCalibData(args.c, mtx, dist)
    elif args.c != None and args.p == None:
        mtx, dist = loadCalibData(args.c)

    # Set threshold values
    thresh = {
              'sobel' : (20, 255),
              'hue_y' : (17, 30),
              'sat_y' : (30, 255),
              'light_y' : (0, 255),
              'hue_w' : (0, 255),
              'sat_w' : (180, 255),
              'light_w' : (180, 255),
              }
    '''
    thresh = {
              'hue_y' : (18, 30),
              'sat_y' : (20, 255),
              'light_y' : (160, 255),
              'hue_w' : (100, 110),
              'sat_w' : (10, 60),
              'light_w' : (130, 255),
              'hue_w2' : (0, 180),
              'sat_w2' : (0, 255),
              'light_w2' : (200, 255)}
    '''

    # This is temporary, will be removed after debugging
    # Read test image
    if args.i != None:
        img = cv2.imread(args.i)
        # Undistort image
        img_undst = cv2.undistort(img, mtx, dist, None, mtx)
        # Apply color and gradient threshold to the image
        img_bin = convertToBinary(img_undst, thresh)
        # Plot the result
        f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
        f.tight_layout()
        ax1.imshow(img)
        ax1.set_title('Original Image', fontsize=25)
        ax2.imshow(img_bin, cmap='gray')
        ax2.set_title('Binary Image', fontsize=25)
        plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)
        plt.show()

    # Read video
    if args.v != None:
        print(args.v)
        video = VideoFileClip(args.v)
        cv2.namedWindow("Output", flags=cv2.WINDOW_AUTOSIZE)

        for frame in video.iter_frames():
            # Undistort frame
            frame_undst = cv2.undistort(frame, mtx, dist, None, mtx)
            # Binarize frame
            frame_bin = convertToBinary(frame_undst, thresh)
            # Display
            cv2.imshow("Output", frame_bin)
            cv2.waitKey(1)
