%% Generate rotation and translation matrices for different orientation
% Load intrinsic.mat and teapot.mat
load('intrinsics.mat');
load('teapot.mat');

% Translation Vectors for camera
t1 = [0, -10, 1]';
t2 = [0,0,11]';
t3 = [-10, 0, 1]';
t4 = [4.5, -4.5, 7.7782]';

% Rotation Matrices for camera
R1 = [1, 0, 0; 0, cosd(-90), -sind(-90); 0, sind(-90), cosd(-90)]

R2 = R1 * [1, 0, 0; 0, cosd(-90), -sind(-90); 0, sind(-90), cosd(-90)]

R3 = R1 * [cosd(90), 0, sind(90); 0, 1, 0; -sind(90), 0, cosd(90)]

R4 = R1 * [cosd(-45), 0, sind(-45); 0, 1, 0; -sind(-45), 0, cosd(-45)] * ...
     [1, 0, 0; 0, cosd(-45), -sind(-45); 0, sind(-45), cosd(-45)]

 
% Plotting
% Plot the teapot points in 3D
plot3(Str(1,:), Str(2,:), Str(3,:));
hold on;
% Show camera orientation
% Note: plotCamera is part of 'Computer Vision System Toolbox'
% Note: Since we want to view the world frame from a camera, I am passing
% transpose of rotation matrices(cRw), to get correct representation in 3D
% plot.
cam1 = plotCamera('Location',t1','Orientation',R1','Opacity',0.3,'Label','Cam1','AxesVisible',true);
hold on;
cam2 = plotCamera('Location',t2','Orientation',R2','Opacity',0.3,'Label','Cam2','AxesVisible',true);
hold on;
cam3 = plotCamera('Location',t3','Orientation',R3','Opacity',0.3,'Label','Cam3','AxesVisible',true);
hold on;
cam4 = plotCamera('Location',t4','Orientation',R4','Opacity',0.3,'Label','Cam4','AxesVisible',true);
hold on;
grid on;
axis equal;
xlim([-15,15]);
ylim([-15,15]);
zlim([0,15]);


% Show projected images from different camera locations
figure(2)
projectedImg = project(Str, K, R1, t1);
imshow(projectedImg)

figure(3)
projectedImg = project(Str, K, R2, t2);
imshow(projectedImg)

figure(4)
projectedImg = project(Str, K, R3, t3);
imshow(projectedImg)

figure(5)
projectedImg = project(Str, K, R4, t4);
imshow(projectedImg)



%% Function to generate projected Image
function [projImg] = project(Str, K, R, t)
    % Calculate Transformation matrix
    T = [R' -R'*t];
    % Calculate Projection matrix
    PMatrix = K * T;
    
    % Generate Black Background Image
    projImg = zeros(480, 640);
    
    % Calculate projected points and mark them on the black background
    for i=1:size(Str, 2)
       point = PMatrix * vertcat(Str(:,i),1);
       projImg(ceil(point(2)/point(3)), ceil(point(1)/point(3))) = 255;
    end
    % Cast the image to uint8
    projImg = uint8(projImg);
end