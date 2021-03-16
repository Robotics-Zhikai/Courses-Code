function out = LineBlurAvg(grayimage,normvector,pixvalue,interval)
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