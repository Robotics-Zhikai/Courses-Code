function im = ResizeImageTimesOf(im,widthtimes,heighttimes)
    originWidth = size(im,2);
    originHeight = size(im,1);
    
    if mod(originWidth,widthtimes)==0 && mod(originHeight,heighttimes)==0
        im = im;
    else
        newbiWidth = floor((originWidth + widthtimes-1) / widthtimes) * widthtimes;
        newbiHeight = floor((originHeight + heighttimes-1) / heighttimes) * heighttimes;
        imagenew = zeros(newbiHeight,newbiWidth);
        imagenew(1:originHeight,1:originWidth) = im;
        im = imagenew;
    end
end