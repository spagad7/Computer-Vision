%% Function to reconstruct image from Laplacian pyramid
% Input: Laplacian pyramid of an image
% Output: Reconstructed image
function [im] = reconstructImage(lpyr)
    img = lpyr{1,1};
    for i=2:size(lpyr,2)
        uimg = upsampleImg(img);
        simg = smoothenImg(uimg, 'laplace');
        img = simg + lpyr{1,i};
    end
    im = img;
end


%% Function to smoothen an image based on a binomial kernel
function [simg] = smoothenImg(img, type)
    X = 1/16 * [1, 4, 6, 4, 1];
    if(strcmp(type,'gauss'))
        H = X' * X;
    elseif(strcmp(type,'laplace'))
        H = 4 *(X' * X);
    end
    simg = convn(img, H, 'same');
    simg = im2double(simg);
end


%% Function to upsample an image
function [uimg] = upsampleImg(img)
    [imgRows, imgCols, imgDim] = size(img);
    uimg = zeros(imgRows*2, imgCols*2, imgDim);
    
    iCounter = 1;
    for i=1:2:imgRows*2
        jCounter = 1;
        for j=1:2:imgCols*2
            uimg(i,j,:) = img(iCounter,jCounter,:);
            if(jCounter<=imgCols)
                jCounter = jCounter + 1;
            end
        end
        if(iCounter <= imgRows)
            iCounter = iCounter + 1;
        end
    end
    uimg = im2double(uimg);
end