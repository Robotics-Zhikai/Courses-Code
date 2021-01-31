function imagedeblur = WienerLinearDeblur(imageblur,xa,yb,T,K)
    im = offsetImageFrequency2Center(imageblur);
    im = fft2(im);
    
    a = xa;
    b = yb;
    wienerFilter = GetLinearDeBlurH_wiener(size(im,2),size(im,1),a,b,T,K);
    
    im = im.*wienerFilter;
    im = ifft2(im);
    imagedeblur = RecoverFromCenter(im);
end