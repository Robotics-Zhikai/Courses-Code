function imageblur = RotateBlurFilter(image,omiga,T)
    im = offsetImageFrequency2Center(image);
    imagefft = fft2(im);
    
    Filter = GetRotateBlurH(size(im,2),size(im,1),omiga,T);
    
    imagefft = imagefft.*Filter;
    
    im = ifft2(imagefft);
    imageblur = RecoverFromCenter(im);
end