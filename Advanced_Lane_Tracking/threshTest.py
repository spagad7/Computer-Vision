import cv2
import argparse
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg


# Function to get color based threshold of an image
def colorThreshold(img, channel, thresh=(0, 255)):
    img_ch = img

    if channel == 'r' or channel == 'g' or channel == 'b':
        if(channel == 'r'):
            img_ch = img[:,:,0]
        elif(channel == 'g'):
            img_ch = img[:,:,1]
        elif(channel == 'b'):
            img_ch = img[:,:,2]

    elif channel == 'h' or channel == 'l' or channel == 's':
        img_hls = cv2.cvtColor(img, cv2.COLOR_BGR2HLS)
        if(channel == 'h'):
            img_ch = img_hls[:,:,0]
        elif(channel == 'l'):
            img_ch = img_hls[:,:,1]
        elif(channel == 's'):
            img_ch = img_hls[:,:,2]

    img_scaled = np.uint8((img_ch * 255)/np.max(img_ch))

    img_thresh = np.zeros_like(img_scaled)
    img_thresh[(img_scaled > thresh[0]) & (img_scaled <= thresh[1])] = 255
    return  img_thresh


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


# Parse Arguments
parser = argparse.ArgumentParser()
parser.add_argument('img_path',
                    type=str,
                    help='path to image file')
parser.add_argument('channel',
                    type=str,
                    help='color channel [r, g, b, h, l, s]')
parser.add_argument('t_low',
                    type=float,
                    help='lower threshold')
parser.add_argument('t_high',
                    type=float,
                    help='higher threshold')
args = parser.parse_args()

img = mpimg.imread(args.img_path)
##img_thresh = colorThreshold(img, args.channel, (args.t_low, args.t_high))
img_thresh = gradientSobel(img, thresh=(args.t_low, args.t_high))
#img_thresh = gradientMag(img, thresh=(args.t_low, args.t_high))
#img_thresh = gradientDir(img, thresh=(args.t_low, args.t_high))
#img_thresh = colorThreshold(img, args.channel, (args.t_low, args.t_high))

# Plot the result
f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
f.tight_layout()
ax1.imshow(img)
ax1.set_title('Original Image', fontsize=25)
ax2.imshow(img_thresh, cmap='gray')
ax2.set_title('Color Threshold', fontsize=25)
plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)
plt.show()
