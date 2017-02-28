% Function to calculate gradient of image
% Input: image (color/grayscale)
% Output: 2 imgages, gradient magnitude and gradient direction
function [Gm, Gd] = mygradient(img)    
    % Convert the image to double
    if(isa(img, 'uint8'))
        img = im2double(img);
    end
    
    % Uncomment this part to show original image
    % figure, imshow(img), title('Original')
    
    % Convert the image to gray scale
    if(size(img, 3) > 1)
        img = rgb2gray(img);
    end
    
    pKernel = [-1, 0, 1; -1, 0, 1; -1, 0, 1];
    Gx = conv2(img, pKernel, 'same');
    Gy = conv2(img, pKernel', 'same');
    Gm = sqrt(Gx.^2 + Gy.^2);
    Gd = atan2(Gy, Gx);
    
    % Uncomment this part to show X and Y direction Image
    % figure, imshow(Gx), title('X-Direction Image')
    % figure, imshow(Gy), title('Y-Direction Image')

    % Uncomment this part to show the images
    % figure, subplot(1,5,1), imshow(img), title('Original')
    % subplot(1,5,2), imshow(Gx), title('Gx')
    % subplot(1,5,3), imshow(Gy), title('Gy')
    % subplot(1,5,4), imshow(Gm), title('Magnitude')
    % subplot(1,5,5), imshow(Gd), title('Direction')
end