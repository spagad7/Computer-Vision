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
    parser.add_argument('img_path',
                        type=str,
                        help='path to image file')
    args = parser.parse_args()

    # Read image
    img = cv2.imread(args.img_path)
    img_grad_x = gradient_sobel(img, orient='x', k_size=3, thresh=(20,100))
    #cv2.imshow("Gradient X", img_grad_x)
    #cv2.waitKey(0)

    img_grad_y = gradient_sobel(img, orient='y', k_size=3, thresh=(20,100))
    #cv2.imshow("Gradient Y", img_grad_y)
    #cv2.waitKey(0)

    img_grad_mag = gradient_mag(img, k_size=3, thresh=(20,100))
    #cv2.imshow("Gradient Magnitude", img_grad_mag)
    #cv2.waitKey(0)

    img_grad_dir = gradient_dir(img, k_size=3, thresh=(0.7,1.3))
    cv2.imshow("Gradient Direction", img_grad_dir)
    cv2.waitKey(0)


'''
    grad_binary = gradient_dir(img, k_size=3, thresh=(0.3,1.3))
    # Plot the result
    f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
    f.tight_layout()
    ax1.imshow(img)
    ax1.set_title('Original Image', fontsize=50)
    ax2.imshow(grad_binary, cmap='gray')
    ax2.set_title('Thresholded Gradient', fontsize=50)
    plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)
    plt.show()
'''
