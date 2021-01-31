function value = GetFucValue_TwoVars(fucstr,x,y,direct,absSmall)
    %双变量x y的求值
    %direct表示当给fucsym除以一个0时，是给x y 加一个无穷小量还是减一个无穷小量
    %fucstr = f(x,y) 是string类型
    try
        value = eval(fucstr); %因为是多变量 所以只能用eval str
    catch
        if direct=='+'
            x = x+absSmall; %说明分母为0，除了一个0，出现错误
            y = y+absSmall;
        elseif direct=='-'
            x = x-absSmall;
            y = y-absSmall;
        else
            error('程序写错了，direct要不等于+ 要不等于-')
        end
        try
            value = eval(fucstr);
        catch
            error('程序无法处理这种程度的未定义函数值');
        end
    end
    
    if isinf(value)
        error('目前处理不了这种情况，被积函数在某一点的值都成了无穷了，函数是无界的')
    end
end