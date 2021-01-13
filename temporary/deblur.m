clc
clear
close all

syms t T r omiga 
f = exp(-r*2*pi*1i*(cos(omiga*t)+sin(omiga*t)));
f = exp(-r*2*pi*1i*(1-0.5*(omiga*t)^2+omiga*t));%等价无穷小
int(f,t,0,T)


image =imread('ex2_origin_gray8.bmp');
image = double(image);
originWidth = size(image,2);
originHeight = size(image,1);
image = ResizeImageTimesOf(image,2*2^(ceil(log2(originWidth))),2*2^(ceil(log2(originHeight))));

for y=1:size(image,1)
    for x = 1:size(image,2)
        image(y,x)=image(y,x)*(-1)^(x+y);
    end
end

imagefft = fft2(image);

width = size(image,2);
height = size(image,1);

Filter = GetLinearBlurH(width,height,0.01,0,1);

imagefft = imagefft.*Filter;

image = ifft2(imagefft);

for y=1:size(image,1)
    for x = 1:size(image,2)
        image(y,x) = real(image(y,x))*(-1)^(x+y);
    end
end

image = uint8(image);
imshow(image)

function image = ResizeImageTimesOf(image,widthtimes,heighttimes)
    originWidth = size(image,2);
    originHeight = size(image,1);
    
    if mod(originWidth,widthtimes)==0 && mod(originHeight,heighttimes)==0
        image = image;
    else
        newbiWidth = floor((originWidth + widthtimes-1) / widthtimes) * widthtimes;
        newbiHeight = floor((originHeight + heighttimes-1) / heighttimes) * heighttimes;
        imagenew = zeros(newbiHeight,newbiWidth);
        imagenew(1:originHeight,1:originWidth) = image;
        image = imagenew;
    end
end

function result = sinxDividex(x)
    if (abs(x) < 0.000000001)
        result = 1;
    else
        result = sin(x) / x;
    end
end

function Filter = GetLinearBlurH(width,height,a,b,T)
    Filter = zeros(size(image,1),size(image,2));
    for v=1:height
        for u = 1:width
            uv = pi*(a*(u-1 - width / 2.0) + b*(v-1 - height / 2.0));
            Filter(v,u) = T * sinxDividex(uv) * exp(-1i*uv);
        end
    end
end






