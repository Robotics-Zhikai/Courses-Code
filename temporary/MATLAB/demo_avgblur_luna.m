clc
clear 
close all

grayimage = imread('ex2_origin_gray8.bmp');

interval = 0.04;
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

imwrite(LineBlur5, 'LineBlur5.bmp');
imwrite (LineBlur10, 'LineBlur10.bmp');
imwrite (LineBlur15, 'LineBlur15.bmp');
imwrite (LineBlur20, 'LineBlur20.bmp');
imwrite (LineBlur25, 'LineBlur25.bmp');
imwrite (LineBlur30, 'LineBlur30.bmp');
imwrite (LineBlur35, 'LineBlur35.bmp');
imwrite (LineBlur40, 'LineBlur40.bmp');
imwrite (LineBlur45, 'LineBlur45.bmp');


interval = 0.04;
rotateblur0 = RotateBlurAvg(grayimage,0,interval);
rotateblur1 = RotateBlurAvg(grayimage,1,interval);
rotateblur2 = RotateBlurAvg(grayimage,2,interval);
rotateblur3 = RotateBlurAvg(grayimage,3,interval);
rotateblur4 = RotateBlurAvg(grayimage,4,interval);
rotateblur5 = RotateBlurAvg(grayimage,5,interval);
rotateblur6 = RotateBlurAvg(grayimage,6,interval);
rotateblur7 = RotateBlurAvg(grayimage,7,interval);
rotateblur8 = RotateBlurAvg(grayimage,8,interval);
rotateblur10 = RotateBlurAvg(grayimage,10,interval);
rotateblur15 = RotateBlurAvg(grayimage,15,interval);
rotateblur20 = RotateBlurAvg(grayimage,20,interval);

imwrite(rotateblur0, 'rotateblur0.bmp');
imwrite (rotateblur1, 'rotateblur1.bmp');
imwrite (rotateblur2, 'rotateblur2.bmp');
imwrite (rotateblur3, 'rotateblur3.bmp');
imwrite (rotateblur4, 'rotateblur4.bmp');
imwrite (rotateblur5, 'rotateblur5.bmp');
imwrite (rotateblur6, 'rotateblur6.bmp');
imwrite (rotateblur7, 'rotateblur7.bmp');
imwrite (rotateblur8, 'rotateblur8.bmp');
imwrite (rotateblur10, 'rotateblur10.bmp');
imwrite (rotateblur15, 'rotateblur15.bmp');
imwrite (rotateblur20, 'rotateblur20.bmp');


% figure
% subplot(241)
% imshow()
% subplot(242)
% imshow(RotateBlur(grayimage,3,0.04))
% subplot(243)
% imshow(RotateBlur(grayimage,4,0.04))
% subplot(244)
% imshow(RotateBlur(grayimage,5,0.04))
% subplot(245)
% imshow(RotateBlur(grayimage,6,0.04))
% subplot(246)
% imshow(RotateBlur(grayimage,7,0.04))
% subplot(247)
% imshow(RotateBlur(grayimage,8,0.04))
% subplot(248)
% imshow(RotateBlur(grayimage,9,0.04))




