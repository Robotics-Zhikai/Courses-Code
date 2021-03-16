clc
clear 
close all

grayimage = imread('���ʻ�����.bmp');
grayimage = grayimage(1:size(grayimage,1)/2,1:size(grayimage,2)*0.9,:);

figure
imshow(grayimage)

grayimageR = grayimage(:,:,1);
grayimageG = grayimage(:,:,2);
grayimageB = grayimage(:,:,2);

figure
imshow(grayimageR)
figure
imshow(grayimageG)
figure
imshow(grayimageB)

interval = 0.1;
rotateblurR = RotateBlurAvg(grayimageR,2.5,interval);
rotateblurG = RotateBlurAvg(grayimageG,2.5,interval);
rotateblurB = RotateBlurAvg(grayimageB,2.5,interval);

rotatemerge = [];
rotatemerge(:,:,1) = rotateblurR;
rotatemerge(:,:,2) = rotateblurG;
rotatemerge(:,:,3) = rotateblurB;

figure
imshow(rotatemerge)

grayimage = rgb2gray(grayimage);


interval = 0.1;
% rotateblur0 = RotateBlurAvg(grayimage,0,interval);
% rotateblur1 = RotateBlurAvg(grayimage,1,interval);
% rotateblur2 = RotateBlurAvg(grayimage,2,interval);
rotateblur3 = RotateBlurAvg(grayimage,2.5,interval);
% rotateblur4 = RotateBlurAvg(grayimage,4,interval);
% rotateblur5 = RotateBlurAvg(grayimage,5,interval);
% rotateblur6 = RotateBlurAvg(grayimage,6,interval);
% rotateblur7 = RotateBlurAvg(grayimage,7,interval);
% rotateblur8 = RotateBlurAvg(grayimage,8,interval);
% rotateblur10 = RotateBlurAvg(grayimage,10,interval);
% rotateblur15 = RotateBlurAvg(grayimage,15,interval);
% rotateblur20 = RotateBlurAvg(grayimage,20,interval);

% imwrite(rotateblur0, '���ʻ�����rotateblur0.bmp');
% imwrite (rotateblur1, '���ʻ�����rotateblur1.bmp');
% imwrite (rotateblur2, '���ʻ�����rotateblur2.bmp');
imwrite (rotateblur3, '���ʻ�����rotateblur2.5.bmp');
% imwrite (rotateblur4, '���ʻ�����rotateblur4.bmp');
% imwrite (rotateblur5, '���ʻ�����rotateblur5.bmp');
% imwrite (rotateblur6, '���ʻ�����rotateblur6.bmp');
% imwrite (rotateblur7, '���ʻ�����rotateblur7.bmp');
% imwrite (rotateblur8, '���ʻ�����rotateblur8.bmp');
% imwrite (rotateblur10, '���ʻ�����rotateblur10.bmp');
% imwrite (rotateblur15, '���ʻ�����rotateblur15.bmp');
% imwrite (rotateblur20, '���ʻ�����rotateblur20.bmp');


interval = 1;
vector = [1,0];
LineBlur5 = LineBlurAvg(grayimage,vector,5,interval);
LineBlur10 = LineBlurAvg(grayimage,vector,10,interval);
LineBlur15 = LineBlurAvg(grayimage,vector,15,interval);
LineBlur20 = LineBlurAvg(grayimage,vector,20,interval);
LineBlur25 = LineBlurAvg(grayimage,vector,25,interval);
LineBlur30 = LineBlurAvg(grayimage,vector,30,interval);
LineBlur35 = LineBlurAvg(grayimage,vector,35,interval);
LineBlur40 = LineBlurAvg(grayimage,vector,40,interval);
LineBlur45 = LineBlurAvg(grayimage,vector,45,interval);

imwrite(LineBlur5, '���ʻ�����LineBlur5.bmp');
imwrite (LineBlur10, '���ʻ�����LineBlur10.bmp');
imwrite (LineBlur15, '���ʻ�����LineBlur15.bmp');
imwrite (LineBlur20, '���ʻ�����LineBlur20.bmp');
imwrite (LineBlur25, '���ʻ�����LineBlur25.bmp');
imwrite (LineBlur30, '���ʻ�����LineBlur30.bmp');
imwrite (LineBlur35, '���ʻ�����LineBlur35.bmp');
imwrite (LineBlur40, '���ʻ�����LineBlur40.bmp');
imwrite (LineBlur45, '���ʻ�����LineBlur45.bmp');








