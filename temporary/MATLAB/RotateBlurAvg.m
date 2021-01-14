function out = RotateBlurAvg(grayimage,rotatedegree,degreeInterval)
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
