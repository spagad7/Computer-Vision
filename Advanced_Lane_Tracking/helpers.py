#!/usr/bin/python3
import cv2
import numpy as np


# Function to get gradient of an image along x or y direction
def gradient_sobel(img, orient='x', k_size=3, thresh=(0,255)):
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
def gradient_mag(img, k_size=3, thresh=(0,255)):
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
def gradient_dir(img, k_size=3, thresh=(0, np.pi/2)):
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    sobelX = cv2.Sobel(img_gray, cv2.CV_64F, 1, 0, ksize=k_size)
    sobelY = cv2.Sobel(img_gray, cv2.CV_64F, 0, 1, ksize=k_size)
    sobelX_abs = np.absolute(sobelX)
    sobelY_abs = np.absolute(sobelY)
    sobel_dir = np.arctan2(sobelY_abs, sobelX_abs)
    sobel_thresh = np.zeros_like(sobel_dir)
    sobel_thresh[(sobel_dir >= thresh[0]) & (sobel_dir <= thresh[1])] = 255
    return sobel_thresh
