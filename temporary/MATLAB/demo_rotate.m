clc
clear
close all
%���̫���� �����Ȱ�ԭ�ƻ��߰�

im =imread('ex2_origin_gray8.bmp');
im = imresize(im,[50,50]);
imshow(im)
im = double(im);


imageblur = RotateBlurFilter(im,0.001,1);
% GetRotateBlurH(20,20,0.001,1);


Extendedimage = ExtendImage(im);
% Extendedimage = im;%����ЧӦ������

a = 0.03;
b=0;
T = 1;
imageblur = LinearBlur(Extendedimage,a,b,T);

figure
imshow(uint8(imageblur)) %����ģ����ͼ��

K = 0.01;
imagedeblur = WienerLinearDeblur(imageblur,a,b,T,K);

figure
imshow(uint8(imagedeblur)) %����ȥģ����ͼ��

