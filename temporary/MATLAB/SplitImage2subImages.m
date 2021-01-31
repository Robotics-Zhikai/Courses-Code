function subimages = SplitImage2subImages(im,subwidth,subheight)
    if mod(size(im,1),subheight)~=0||mod(size(im,2),subwidth)~=0
        error('Ӧ�ñ�֤�����im��subwidth��subheight�ı���')
    end
    subimages = [];
    for x=0:size(im,2)/subwidth-1
        for y = 0:size(im,1)/subheight-1
            subimages{y+1,x+1} = im(y*subheight+1:(y+1)*subheight,x*subwidth+1:(x+1)*subwidth);
        end
    end
end