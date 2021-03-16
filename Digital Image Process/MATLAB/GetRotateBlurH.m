function Filter = GetRotateBlurH(width,height,omiga,T)
    H = zeros(height,width);
    syms t
    for v=1:height
        for u = 1:width
            [u v]
            utmp = u-width/2;
            vtmp = v-height/2;  
            r = ((utmp*1/2834)^2+(vtmp*1/2834)^2)^0.5;

%             f = exp(-r*2*pi*1i*(utmp*cos(omiga*t)+vtmp*sin(omiga*t)));
            f = exp(-r*2*pi*1i*(utmp*(1-0.5*(omiga*t)^2)+vtmp*omiga*t));
            huv =vpa(int(f,t,0,T));
            H(v,u) = huv;
        end
    end
    Filter = H;
end