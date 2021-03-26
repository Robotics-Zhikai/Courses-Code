clc
clear
close all

syms t T r omiga u v
f = exp(-r*2*pi*1i*(u*cos(omiga*t)+v*sin(omiga*t)));
f = exp(-r*2*pi*1i*(1-0.5*(omiga*t)^2+omiga*t));%等价无穷小
int(f,t,0,T)



im =imread('ex2_origin_gray8.bmp');
im = double(im);

Extendedimage = ExtendImage(im);
% Extendedimage = im;%波纹效应更严重

a = 0.03;
b=0.03;
T = 1;
imageblur = LinearBlurFilter(Extendedimage,a,b,T);

figure
imshow(uint8(imageblur)) %线性模糊的图像

K = 0.01;
imagedeblur = WienerLinearDeblur(imageblur,a,b,T,K);

figure
imshow(uint8(imagedeblur)) %线性去模糊的图像





































