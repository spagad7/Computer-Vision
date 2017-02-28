% Function to generate edge image using canny edge detector method
% Input: image(color/grayscale)
% Output: edge image
function edgeImg = mycanny(img)
    % Convert image to double
    if(isa(img, 'uint8'))
        img = im2double(img);
    end
    
    % Convert the image to gray scale
    if(size(img, 3) > 1)
        img = rgb2gray(img);
    end
    
    % Uncomment to show original image
    % figure, imshow(img), title('Original Image');
    
    % Calculate Gaussian Kernel
    G = fspecial('gaussian', [3, 3], 1);
    
    % Smoothen Image
    simg = convn(img, G, 'same');
    % Uncomment to show Filtered image
    % figure, imshow(simg), title('Filtered Image');
    
    % Calculate and Magnitude and Direction
    [Gm, Gd] = mygradient(simg);
    % Uncomment to show gradient magnitude and direction image
    % figure, imshow(Gm), title('Magnitude')
    % figure, imshow(Gd), title('Direction')
    
    % Non max suppression
    nmGm = nonMaxSuppress(Gm, Gd);
    % Uncomment to show non-max suppression image image
    % figure, imshow(nmGm), title('Non-max Suppression')
    
    % Hysterisis thresholding
    edgeImg = hystThreshold(nmGm);
    % Uncomment to show final image
    % figure, imshow(edgeImg), title('My Canny Edge')
    
end

% Function to perform nonmax suppression
function [nmGm] = nonMaxSuppress(Gm, Gd)
    nmGm = zeros(size(Gm));
    for i=1:size(Gm,1)
        for j=1:size(Gm,2)
            % Get pixel angle
            angle = Gd(i,j);
            % Get neighboring pixels from Gd value
            if((angle<=degtorad(22.5) && angle>=degtorad(0)) ...
                    || (angle>=degtorad(-22.5) && angle<degtorad(0)) ...
                    || (angle>=degtorad(157.5) && angle<=degtorad(180)) ...
                    || (angle<degtorad(-157.5) && angle>=degtorad(-180)))
                nextX = i;
                nextY = j+1;
                previousX = i;
                previousY = j-1;
            elseif((angle>degtorad(22.5) && angle<=degtorad(67.5)) ...
                    || (angle<degtorad(-112.5) && angle>=degtorad(-157.5)))
                nextX = i-1;
                nextY = j+1;
                previousX = i+1;
                previousY = j-1;
            elseif((angle>degtorad(67.5) && angle<=degtorad(112.5)) ...
                    || (angle<degtorad(-67.5) && angle>=degtorad(-112.5)))
                nextX = i+1;
                nextY = j;
                previousX = i-1;
                previousY = j;
            elseif((angle>degtorad(112.5) && angle<=degtorad(157.5)) ...
                    || (angle<degtorad(-22.5) && angle>=degtorad(-67.5)))
                nextX = i+1;
                nextY = j+1;
                previousX = i-1;
                previousY = j-1;
            else
                radtodeg(angle)
            end
            
            % Check boundary conditions
            if((nextX >= 1 && nextX <= size(Gm,1)) && (previousX >= 1 && previousX <= size(Gm,1)) ...
                    && (nextY >= 1 && nextY <= size(Gm,2)) && (previousY >= 1 && previousY <= size(Gm,2)))
                
                if(Gm(nextX,nextY) >= Gm(i,j) || Gm(previousX, previousY) >= Gm(i,j))
                    nmGm(i,j) = 0;
                else
                    nmGm(i,j) = Gm(i,j);
                end
            end
        end
    end
end


%% Function to perform hysterisis thresholding
function [himg] = hystThreshold(nmImg)
    high = 2.0;
    low = 0.3;
    
    for i=2:size(nmImg,1)-1
        for j=2:size(nmImg,2)-1
            if(nmImg(i,j) > high)
                continue;
            elseif(nmImg(i,j) < low)
                nmImg(i,j) = 0;
            elseif(nmImg(i,j)>=low && nmImg(i,j)<=high)
                % Right
                if(nmImg(i,j+1) > high)
                    nmImg(i,j) = 255;
                    continue;
                % Left    
                elseif(nmImg(i,j-1) > high)
                    nmImg(i,j) = 255;
                    continue;
                % Top
                elseif(nmImg(i-1,j) > high)
                    nmImg(i,j) = 255;
                    continue;
                % Bottom
                elseif(nmImg(i+1,j) > high)
                    nmImg(i,j) = 255;
                    continue;
                % Diagonal Top Right
                elseif(nmImg(i-1,j+1) > high)
                    nmImg(i,j) = 255;
                    continue;
                % Diagonal Top Left
                elseif(nmImg(i-1,j-1) > high)
                    nmImg(i,j) = 255;
                    continue;
                % Diagonal Bottom Right
                elseif(nmImg(i+1,j+1) > high)
                    nmImg(i,j) = 255;
                    continue;
                % Diagonal Bottom left
                elseif(nmImg(i+1,j-1) > high)
                    nmImg(i,j) = 255;
                    continue;
                end
            end    
        end
    end
    himg = nmImg;
end