#!/usr/bin/python3
import cv2
import glob
import numpy as np


# Function to get gradient of an image along x or y direction
def gradientSobel(img, orient='x', k_size=3, thresh=(0,255)):
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    if(orient == 'x'):
        sobel = cv2.Sobel(img_gray, cv2.CV_64F, 1, 0, ksize=k_size)
    elif(orient == 'y'):
        sobel = cv2.Sobel(img_gray, cv2.CV_64F, 0, 1, ksize=k_size)
    sobel_abs = np.absolute(sobel)
    sobel_scale = np.uint8((sobel_abs * 255)/np.max(sobel_abs))
    sobel_thresh = np.zeros_like(sobel_scale)
    sobel_thresh[(sobel_scale >= thresh[0]) & (sobel_scale <= thresh[1])] = 255
    return sobel_thresh


# Function to get gradient magnitude of an image
def gradientMag(img, k_size=3, thresh=(0,255)):
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    sobelX = cv2.Sobel(img_gray, cv2.CV_64F, 1, 0, ksize=k_size)
    sobelY = cv2.Sobel(img_gray, cv2.CV_64F, 0, 1, ksize=k_size)
    sobelX_abs = np.absolute(sobelX)
    sobelY_abs = np.absolute(sobelY)
    sobel_mg = np.sqrt(np.square(sobelX_abs) + np.square(np.square(sobelY_abs)))
    sobel_scaled = np.uint8((sobel_mg * 255)/np.max(sobel_mg))
    sobel_thresh = np.zeros_like(sobel_scaled)
    sobel_thresh[(sobel_scaled >= thresh[0]) & (sobel_scaled <= thresh[1])] = 255
    return sobel_thresh


# Function to get gradient direction of an image
def gradientDir(img, k_size=3, thresh=(0, np.pi/2)):
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    sobelX = cv2.Sobel(img_gray, cv2.CV_64F, 1, 0, ksize=k_size)
    sobelY = cv2.Sobel(img_gray, cv2.CV_64F, 0, 1, ksize=k_size)
    sobelX_abs = np.absolute(sobelX)
    sobelY_abs = np.absolute(sobelY)
    sobel_dir = np.arctan2(sobelY_abs, sobelX_abs)
    sobel_thresh = np.zeros_like(sobel_dir)
    sobel_thresh[(sobel_dir >= thresh[0]) & (sobel_dir <= thresh[1])] = 255
    return sobel_thresh


# Function to get color based threshold of an image
# The s channel in HLS is good for identifying staturated bright yellow lanes
# The l channel in HLS is good for identifying very bright white lanes
def colorThreshold(img, thresh):
    img_hls = cv2.cvtColor(img, cv2.COLOR_BGR2HLS)
    img_h = img_hls[:,:,0]
    img_l = img_hls[:,:,1]
    img_s = img_hls[:,:,2]

    img_thresh = np.zeros_like(img_s)
    cond_y_h = (img_h > thresh['hue_y'][0]) & (img_h <= thresh['hue_y'][1])
    cond_y_l = (img_l > thresh['light_y'][0]) & (img_l <= thresh['light_y'][1])
    cond_y_s = (img_s > thresh['sat_y'][0]) & (img_s <= thresh['sat_y'][1])
    cond_w_h = (img_h > thresh['hue_w'][0]) & (img_h <= thresh['hue_w'][1])
    cond_w_l = (img_l > thresh['light_w'][0]) & (img_l <= thresh['light_w'][1])
    cond_w_s = (img_s > thresh['sat_w'][0]) & (img_s <= thresh['sat_w'][1])
    cond_w2_h = (img_h > thresh['hue_w2'][0]) & (img_h <= thresh['hue_w2'][1])
    cond_w2_l = (img_l > thresh['light_w2'][0]) & (img_l <= thresh['light_w2'][1])
    cond_w2_s = (img_s > thresh['sat_w2'][0]) & (img_s <= thresh['sat_w2'][1])
    img_thresh[(cond_y_h & cond_y_l & cond_y_s) |
                (cond_w_h & cond_w_l & cond_w_s) |
                (cond_w2_h & cond_w2_l & cond_w2_s)] = 255
    return  img_thresh


# Function to convert 3 channel image to binary image using gradient and color
# based thresholding
def convertToBinary(img, thresh):
    #img_sobel = gradientSobel(img, thresh['sobel'])
    #img_mag = gradientMag(img, thresh['mag'])
    #img_dir = gradientDir(img, thresh['dir'])
    #img_color = colorThreshold(img, thresh['color'])

    img_color = colorThreshold(img, thresh)
    return img_color
