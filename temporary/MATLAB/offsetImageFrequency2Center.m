function im = offsetImageFrequency2Center(im)
    for y=1:size(im,1)
        for x = 1:size(im,2)
            im(y,x)=im(y,x)*(-1)^(x+y);
        end
    end
end