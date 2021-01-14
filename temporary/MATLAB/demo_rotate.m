clc
clear
close all
%这个太慢了 还是先按原计划走吧

im =imread('ex2_origin_gray8.bmp');
im = imresize(im,[50,50]);
imshow(im)
im = double(im);


imageblur = RotateBlurFilter(im,0.001,1);
% GetRotateBlurH(20,20,0.001,1);


Extendedimage = ExtendImage(im);
% Extendedimage = im;%波纹效应更严重

a = 0.03;
b=0;
T = 1;
imageblur = LinearBlur(Extendedimage,a,b,T);

figure
imshow(uint8(imageblur)) %线性模糊的图像

K = 0.01;
imagedeblur = WienerLinearDeblur(imageblur,a,b,T,K);

figure
imshow(uint8(imagedeblur)) %线性去模糊的图像

