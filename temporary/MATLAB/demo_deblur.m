clc
clear
close all

im = imread('概率机器人模糊实拍.bmp'); 
im = imresize(im,[1024,1024]);
figure
imshow(im)
im = double(im);

DeblurImage = RotateDeblurSplitWiener(im,211,211,0.0555,3780,3780);
figure
imshow(uint8(DeblurImage))


im = imread('rotateblur2.bmp');
figure
imshow(im)
im = double(im);

DeblurImage = RotateDeblurSplitWiener(im,21,21,0.0405,2834,2834);
figure
imshow(uint8(DeblurImage))


im = imread('rotateblur3.bmp');
figure
imshow(im)
im = double(im);

% DeblurImage = RotateDeblurSplitWiener(im,11,11,0.40,2834,2834);
% figure
% imshow(uint8(DeblurImage))

Extendedimage = ExtendImage(im);
imshow(uint8(Extendedimage))

xa = -0.0060;
yb = -0.0060;
T = 1;
K = 0.01;
imagedeblur = WienerLinearDeblur(Extendedimage,xa,yb,T,K);

figure
imshow(uint8(imagedeblur)) %线性去模糊的图像


im = imread('rotateblur5.bmp');
figure
imshow(im)
im = double(im);

% DeblurImage = RotateDeblurSplitWiener(im,11,11,0.40,2834,2834);
% figure
% imshow(uint8(DeblurImage))

Extendedimage = ExtendImage(im);
imshow(uint8(Extendedimage))

xa = -0.0105;
yb = -0.0105;
T = 1;
K = 0.01;
imagedeblur = WienerLinearDeblur(Extendedimage,xa,yb,T,K);

figure
imshow(uint8(imagedeblur)) %线性去模糊的图像


im = imread('概率机器人模糊实拍.bmp'); 
im = imresize(im,[1024,1024]);
figure
imshow(im)
im = double(im);

% DeblurImage = RotateDeblurSplitWiener(im,51,51,0.20,3780,3780);
% figure
% imshow(uint8(DeblurImage))

Extendedimage = ExtendImage(im);
imshow(uint8(Extendedimage))

figure
value = 0.014;
vector = [-1 -15];
% value = 0.01;
% vector = [-5 -3];
    
vector = vector/norm(vector);
vector = value*vector;
xa = vector(1);
yb = vector(2);

% xa = -0.00100;
% yb = -0.01500;
T = 1;
K = 0.009;
imagedeblur = WienerLinearDeblur(Extendedimage,xa,yb,T,K);


imshow(uint8(imagedeblur)) %线性去模糊的图像
%     value = value-0.001;





%概率机器人3780/m
%luna 2834/m








im = imread('LineBlur30.bmp');
figure
im = double(im);
% im(400:512,:) = [];

Extendedimage = ExtendImage(im);
imshow(uint8(Extendedimage))

xa = 0.030;
yb = 0;
T = 1;
K = 0.01;
imagedeblur = WienerLinearDeblur(Extendedimage,xa,yb,T,K);

figure
imshow(uint8(imagedeblur)) %线性去模糊的图像
