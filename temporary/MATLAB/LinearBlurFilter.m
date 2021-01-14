function imageblur = LinearBlurFilter(image,xa,yb,T)
    im = offsetImageFrequency2Center(image);

    imagefft = fft2(im);

    a=xa;
    b=yb;
    Filter = GetLinearBlurH(size(im,2),size(im,1),a,b,T);

    imagefft = imagefft.*Filter;

    im = ifft2(imagefft);
    imageblur = RecoverFromCenter(im);
end