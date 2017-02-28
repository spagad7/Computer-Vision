blendImgs.m
===========
This a matlab function to blend apple and orange image into one seemless image. 

blendEye.m
==========
This a matlab function to blend hand and eye into one seemless image. 


Function prototype:
[bpyr] = blendImgs(lpyr1, lpyr2, mask)

Description:
The script first reads the two images and converts them to double format. Next, using the getPyr function from previous question, the Laplacian pyramids for the two images is generated. After this, a mask is created specific to blend half of apple and half of orange image. The mask is of same size as the two images and the pixel values in half of the mask is set to 255 and the pixel values in the remaining half fades to 0. After generating the mask, the two Laplacian images and the mask are passed to the function blendImgs to generate blended pyramid. 

The function blendImgs generates Gaussian pyramid of the mask and generates the blended images at each level of the two Laplacian pyramids  to generate Laplacian pyramid of blended images. The formula used to blend the images is listed below. 

Lfig = Gfig: ∗ L1fig + (1 − Gfig): ∗ L2fig

This formula has been implemented in the function as below, 
Li = lpyr1{1,i} .* gpyr{1, size(gpyr,2)-i+1} ...
             + (255*ones(size(gpyr{1, size(gpyr,2)-i+1})) ...
             - gpyr{1, size(gpyr,2)-i+1}) .* lpyr2{1,i};

After generating the blended pyramid, the blended image is reconstructed by passing the blended Laplacian pyramid to reconstructImg function.




Blending of Hand and Eye
=========================
Blending of hand and eye is similar to the process described above, but the mask used is different. The mask used for blending hand and eye and its Gaussian pyramid is shown below. The images are resized to 256x256 before blending.

The pixels in mask are set to 255 in the region which covers right eye in the eye image. All the other pixels are set to 0. This mask filters only the right eye from the eye image. 
