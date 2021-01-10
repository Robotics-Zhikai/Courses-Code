clc
clear 
close all

grayimage = imread('ex2_origin_gray8.bmp');

interval = 0.04;
vector = [1,0];
LineBlur5 = LineBlur(grayimage,vector,5,interval);
LineBlur10 = LineBlur(grayimage,vector,10,interval);
LineBlur15 = LineBlur(grayimage,vector,15,interval);
LineBlur20 = LineBlur(grayimage,vector,20,interval);
LineBlur25 = LineBlur(grayimage,vector,25,interval);
LineBlur30 = LineBlur(grayimage,vector,30,interval);
LineBlur35 = LineBlur(grayimage,vector,35,interval);
LineBlur40 = LineBlur(grayimage,vector,40,interval);
LineBlur45 = LineBlur(grayimage,vector,45,interval);

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
rotateblur0 = RotateBlur(grayimage,0,interval);
rotateblur1 = RotateBlur(grayimage,1,interval);
rotateblur2 = RotateBlur(grayimage,2,interval);
rotateblur3 = RotateBlur(grayimage,3,interval);
rotateblur4 = RotateBlur(grayimage,4,interval);
rotateblur5 = RotateBlur(grayimage,5,interval);
rotateblur6 = RotateBlur(grayimage,6,interval);
rotateblur7 = RotateBlur(grayimage,7,interval);
rotateblur8 = RotateBlur(grayimage,8,interval);
rotateblur10 = RotateBlur(grayimage,10,interval);
rotateblur15 = RotateBlur(grayimage,15,interval);
rotateblur20 = RotateBlur(grayimage,20,interval);

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



function out = RotateBlur(grayimage,rotatedegree,degreeInterval)
    bigimage = zeros(2*size(grayimage,1),2*size(grayimage,2));
    bigimage(1:size(grayimage,1),size(grayimage,2)+1:2*size(grayimage,2)) = double(grayimage);
%     bigimage = uint8(bigimage);
    RotateSet = [];
    sizeSet = [];
    for i =0:degreeInterval:rotatedegree
        thisimage = imrotate(bigimage,i,'bilinear');
        if (mod(size(thisimage,1),2)==0)
            thisimage(end,:) = [];
        end
        if (mod(size(thisimage,2),2)==0)
            thisimage(:,1) = [];
        end
%         thisimage(size(thisimage,1)/2+1:end,:) = [];
        RotateSet = [RotateSet;{thisimage}];
        sizeSet = [sizeSet;[size(thisimage,1),size(thisimage,2)]];
    end
    
    for i = 1:size(RotateSet,1)
        thisimage = RotateSet{i};
        Background = zeros(max(sizeSet(:,1)),max(sizeSet(:,2)));
        Bgwidth = size(Background,2);
        Bgheight = size(Background,1);
        if (Bgwidth~=size(thisimage,2)&&Bgheight~=size(thisimage,1))
            Background((Bgheight-1)/2-(size(thisimage,1)-1)/2:(Bgheight-1)/2+(size(thisimage,1)-1)/2,(Bgwidth-1)/2-(size(thisimage,2)-1)/2:(Bgwidth-1)/2+(size(thisimage,2)-1)/2) = thisimage;
        else
            Background = thisimage;
        end
        RotateSet{i} = Background;
    end
    
    blurImage = zeros(size(Background,1),size(Background,2));
    for i = 1:size(RotateSet,1)
        blurImage = blurImage + RotateSet{i};
    end
    blurImage = blurImage/size(RotateSet,1);
    
%     for i = 1:size(RotateSet,1)
%         imshow(uint8(RotateSet{i}))
% %         equalzeroRow = find(sum(RotateSet{i},2)==0);
% %         equalzeroCol = find(sum(RotateSet{i},1)==0);
% %         equalzero(end)-equalzero(1)+1
%     end
    uint8blurImage = uint8(blurImage);
    out = uint8blurImage(1:(size(uint8blurImage,1)-1)/2,(size(uint8blurImage,2)-1)/2:end);
%     imshow(out)
end

function out = LineBlur(grayimage,normvector,pixvalue,interval)
    normvector = normvector/norm(normvector);
    vector = normvector*pixvalue;
    transSet = [];
    for i=0:interval:pixvalue
        transSet = [transSet;{imtranslate(grayimage,normvector*i)}];
    end
    
    doubleImage = zeros(size(grayimage,1),size(grayimage,2));
    for i = 1:size(transSet,1)
        doubleImage = doubleImage+double(transSet{i});
    end
    doubleImage = doubleImage/size(transSet,1);
    uint8image = uint8(doubleImage);
%     imshow(uint8image)
    out = uint8image;
end