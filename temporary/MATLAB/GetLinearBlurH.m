function Filter = GetLinearBlurH(width,height,a,b,T) %得到线性模糊的H
    Filter = zeros(height,width);
    for v=1:height
        for u = 1:width
            uv = pi*(a*(u-1 - width / 2.0) + b*(v-1 - height / 2.0));
            Filter(v,u) = T * sinxDividex(uv) * exp(-1i*uv);
        end
    end
end