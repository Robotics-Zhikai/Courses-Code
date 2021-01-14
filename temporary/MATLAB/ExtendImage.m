function Extendedimage = ExtendImage(Image,times)
    im = Image;
    originWidth = size(im,2);
    originHeight = size(im,1);
    if nargin==1
        Extendedimage = ResizeImageTimesOf(im,2*2^(ceil(log2(originWidth))),2*2^(ceil(log2(originHeight))));
    else
        Extendedimage = ResizeImageTimesOf(im,times*2^(ceil(log2(originWidth))),times*2^(ceil(log2(originHeight))));
    end
end

