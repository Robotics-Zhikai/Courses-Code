function xout = BinarySolve(Func_Str,qujian,error)
    qujian = sort(qujian);
    x1 = qujian(1);
    x2 = qujian(2);
    
    x = x1;
    fx1 = eval(Func_Str);
    x = x2;
    fx2 = eval(Func_Str);
    
    if fx1*fx2>0
        error('暂时无法解决这种情况')
    end
    if abs(fx1) <= error
        xout = x1;
        return;
    elseif abs(fx2) <= error
        xout = x2;
        return;
    end
    
    x = (x1+x2)/2;
    fx = eval(Func_Str);
    while abs(fx)>error
        if abs(fx)<=error
            xout = x;
            return;
        end
        if fx*fx1<0
            x2 = x;
        elseif fx*fx2<0
            x1 = x;
        end
        x = x1;
        fx1 = eval(Func_Str);
        x = x2;
        fx2 = eval(Func_Str);
        x = (x1+x2)/2;
        fx = eval(Func_Str);
    end
    xout = x;
end