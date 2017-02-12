function [rotatedImg] = rotateImg (originalImg, angle, P0)
    % Find dimensions of original image
    [imgHeight, imgWidth, imgDim] = size(originalImg);
    % Calculate rotation matrix
    R = [cosd(angle), -sind(angle); sind(angle), cosd(angle)];
    % Create large black image plane
    I1 = zeros(imgHeight*3,imgWidth*3);
    % Map original image on to the large image plane
    for i=1:imgHeight
        for j=1:imgWidth
            I1(i+imgHeight-1, j+imgWidth-1) = originalImg(i,j);
        end
    end
    
    % Find P0 in I1(larger image with black background)
    P0 = [imgHeight, imgWidth] + P0;
    
    % Find corners of the rotated image
    corners = zeros(2,4);
    corners(:,1) = R * ([imgHeight,imgWidth] - P0)' + P0';
    corners(:,2) = R * ([imgHeight,imgWidth*2-1] - P0)' + P0';
    corners(:,3) = R * ([imgHeight*2-1,imgWidth*2-1] - P0)' + P0';
    corners(:,4) = R * ([imgHeight*2-1,imgWidth] - P0)' + P0';
    
    % Find dimensions of the rectangle/matrix which fits the rotated image
    topLeft = [ceil(min(corners(1,:))), ceil(min(corners(2,:)))];
    bottomRight = [ceil(max(corners(1,:))), ceil(max(corners(2,:)))];

    % Calculate Pf. For my code it works with Pf = P0
    Pf = P0;
    
    % Calcutate rotation matrix with angle
    R2 = [cosd(-angle), -sind(-angle); sind(-angle), cosd(-angle)];
    
    % Generate black background with to place rotated image
    I2 = zeros(imgHeight*3,imgWidth*3);
    
    % Map the points in the rectangle back to I1
    for j=topLeft(2):bottomRight(2)
        for i=topLeft(1):bottomRight(1)
            pointI1 = P0' + R2 * ([i,j] - Pf)';
            
            % Bilinear-Interpolation
            if(round(pointI1(1))>=imgHeight && round(pointI1(1))<=imgHeight*2-1 &&...
               round(pointI1(2))>=imgWidth && round(pointI1(2))<=imgWidth*2-1 && ...
                i>=1 && j>=1)
                
                x = pointI1(1);
                y = pointI1(2);
                % Find coordinates of 4 neighboring pixels
                x1 = floor(pointI1(1));
                x2 = ceil(pointI1(1));
                y1 = floor(pointI1(2));
                y2 = ceil(pointI1(2));
                % Form 4 neighbouring pixels
                P1 = [x1,y1];
                P2 = [x2,y1];
                P3 = [x2,y2];
                P4 = [x1,y2];
                
                % Perform Bilinear Interpolation
                % Formula Source: https://en.wikipedia.org/wiki/Bilinear_interpolation
                if(x1 ~= x2 || y1 ~= y2)
                    pixelVal = (1/((x2-x1)*(y2-y1))) * [x2 - x, x-x1] * ...
                    [I1(P1(1),P1(2)), I1(P2(1),P2(2)); I1(P3(1),P3(2)), I1(P4(1),P4(2))] * ...
                    [y2-y;y-y1];
        
                    I2(i,j) = pixelVal;
                else
                    % If pixel coodinates obtained by inverse
                    % transformation equation are integers then they map to
                    % original image pixels correctly, no need to do
                    % bilinear interpolation for this case.
                    I2(i,j) = I1(pointI1(1), pointI1(2));
                end
            end
        end
    end
    % Cast the rotated image matrix to uint8
    rotatedImg = uint8(I2);
end
