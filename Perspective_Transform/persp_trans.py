#!/usr/bin/python3
'''
Steps
=====
1. Calibrate camera
2. Get Perspective Trasnform
3. Draw Chessboard pattern
3. Warp image
'''

import cv2
import argparse
import glob
import numpy as np
import matplotlib.pyplot as plt


if __name__ == '__main__':
    # Parse Arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("dir_path",
                        type=str,
                        help="path to calibration images")
    parser.add_argument("img_path",
                        type=str,
                        help="path to image")
    parser.add_argument("nx",
                        type=int,
                        help="num inner corners along x")
    parser.add_argument("ny",
                        type=int,
                        help="num inner corners along y")
    args = parser.parse_args()

    # Read calibration images
    nx = args.nx
    ny = args.ny
    calib_imgs = glob.glob(args.dir_path + "/*.jpg")
    objp = np.zeros((nx*ny, 3), dtype='float32')
    objp[:,:2] = np.mgrid[0:nx, 0:ny].T.reshape(-1, 2)
    obj_pts = []
    img_pts = []
    img_h = img_w = 0

    # Find Chessboard Pattern in images
    for img_path in calib_imgs:
        img = cv2.imread(img_path)
        img_h = img.shape[0]
        img_w = img.shape[1]
        retval, corners = cv2.findChessboardCorners(img, (nx, ny), None)
        if retval == True:
            obj_pts.append(objp)
            img_pts.append(corners)
            cv2.drawChessboardCorners(img, (nx, ny), corners, retval)
            cv2.imshow("Chessboard Corners", img)
            cv2.waitKey(10)

    # Calibrate camera
    retval, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(obj_pts, img_pts,
                                        (img_w, img_h), None, None)

    # Undistort Image
    img_dst = cv2.imread(args.img_path)
    img_undst = cv2.undistort(img_dst, mtx, dist, None, mtx)

    # Perform perspective transform
    retval, corners = cv2.findChessboardCorners(img_undst, (nx, ny), None)
    if retval==True:
        src_pts = np.array([corners[0], corners[nx-1],
                            corners[-1], corners[-nx]], dtype='float32')
        ofst = 100
        scale = 120
        dst_pts = np.array([(ofst, ofst), (nx*scale+ofst, ofst),
                    (nx*scale+ofst, ny*scale+ofst),
                    (ofst, ny*scale+ofst)], dtype='float32')
        M = cv2.getPerspectiveTransform(src_pts, dst_pts)
        img_warp = cv2.warpPerspective(img_undst, M, img_undst.shape[1::-1],
                                        flags=cv2.INTER_LINEAR)
        #retval, corners = cv2.findChessboardCorners(img_warp, (nx, ny), None)
        #cv2.drawChessboardCorners(img_warp, (nx, ny), corners, retval)
        cv2.imwrite("img_trans.jpg", img_warp)
        cv2.imshow("Transformed Image", img_warp)
        cv2.waitKey(0)
