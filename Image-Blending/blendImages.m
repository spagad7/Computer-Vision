% Script to blend apple and orange image

% Read images
img1 = imread('apple.jpg');
img2 = imread('orange.jpg');

% Convert to double
if(isa(img1, 'uint8'))
    img1 = im2double(img1);
end

if(isa(img2, 'uint8'))
    img2 = im2double(img2);
end

% Generate Laplacian pyramid for both the images
lpyr1 = getPyr(img1, 'laplace', 4);
lpyr2 = getPyr(img2, 'laplace', 4);

% Genreate mask
% This mask produces smoother blend, like the one in pdf, with no sharp lines
mask = zeros(size(img1,1),size(img1,2),3);
mask(:,1:end/2-35,:) = 255;
value = 248;
for i = size(mask,2)/2-35+1:size(mask,2)/2+35
    mask(:,i,:) = value;
    value = value - 3;
end

% Generate blended laplacian pyramid and reconstruct image
bpyr = blendImgs(lpyr1, lpyr2, mask);
rimg = reconstructImage(bpyr);
figure, imshow(uint8(rimg))

%% Function to blend images based on laplacian pyramids and mask
function [bpyr] = blendImgs(lpyr1, lpyr2, mask)
    bpyr = cell(size(lpyr1));
    gpyr = getPyr(mask, 'gauss', 4);
    
    for i=1:size(lpyr1,2)
        % Uncomment this part to show Gaussian pyramid of mask
        % figure, imshow(gpyr{1,i})
        
        % Get weighted average
        Li = lpyr1{1,i} .* gpyr{1, size(gpyr,2)-i+1} ...
             + (255*ones(size(gpyr{1, size(gpyr,2)-i+1})) ...
             - gpyr{1, size(gpyr,2)-i+1}) .* lpyr2{1,i};
        bpyr{1,i} = Li;
        
        % Uncomment this part to show blended pyramid
        % figure, imshow(uint8(Li))
        % Uncomment this part to show pyramid without casting to uint8
        % figure, imshow(Li)
    end
end