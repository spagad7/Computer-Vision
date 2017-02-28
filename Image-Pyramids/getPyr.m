%% Function to get a a cell array consisting of pyramid of images
% Input1: image(color/grayscale)
% Input2: type: gauss/laplace
% Input3: number of levels in pyramid
% Output: pyramid of images in a cell array
function [pyr] = getPyr(img, type, numLevels)
    % Create cell array pyr of size numLevels
    gpyr = cell(1,numLevels);
    lpyr = cell(1,numLevels);
    
    % Convert image to double to prevent loss of information
    if(isa(img, 'uint8'))
        img = im2double(img);
    end
    
    % Gaussian Pyramid
    if(strcmp(type,'gauss'))
        gpyr{1,1} = img;
        for i=2:numLevels
            simg = smoothenImg(img, type);
            dimg = downsampleImg(simg);
            gpyr{1,i} = dimg;
            img = dimg;
        end
        pyr = gpyr;
        
        % Uncomment this part to show images
        % for i=1:numLevels
        %     figure, imshow(gpyr{1,i})
        % end
        
    
    % Laplacian Pyramid    
    elseif(strcmp(type,'laplace'))
        % First, generate Gaussian pyramid
        gpyr{1,1} = img;
        for i=2:numLevels
            simg = smoothenImg(img, 'gauss');
            dimg = downsampleImg(simg);
            gpyr{1,i} = dimg;
            img = dimg;
        end
        % Use Gaussian pyrmaid for constructing Laplacian pyramid
        img = gpyr{1,numLevels};
        lpyr{1,1} = img;
        for i=2:numLevels
            uimg = upsampleImg(img);
            simg = smoothenImg(uimg, type);
            lpyr{1,i} = gpyr{1,numLevels-i+1} - simg;
            img = gpyr{1,numLevels-i+1};
        end
        pyr = lpyr;
        
        % Uncomment this part to show images
        % for i=1:numLevels
        %     figure, imshow(lpyr{1,i})
        % end
    end
end


%% Function to smoothen an image based on a binomial kernel
function [simg] = smoothenImg(img, type)
    X = 1/16 * [1, 4, 6, 4, 1];
    if(strcmp(type,'gauss'))
        H = X' * X;
    elseif(strcmp(type,'laplace'))
        H = 4 * (X' * X);
    end
    simg = convn(img, H, 'same');
    simg = im2double(simg);
end


%% Function to downsample an image
function [dimg] = downsampleImg(img)
    dimg = img(1:2:end, 1:2:end, :);
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