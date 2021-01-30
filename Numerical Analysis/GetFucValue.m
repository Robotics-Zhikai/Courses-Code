function value = GetFucValue(fucsym,num,direct,absSmall)
    %单变量x 的求值
    %direct表示当fucsym中除以一个0时，是给num加一个无穷小量还是减一个无穷小量
    %这里只是为了训练用try catch的思想。实际上由于分母为0的时候是很少数，因此在绝大多数情况下增加了时间量，简单加个if判断0就行
    %没必要用try catch。这样的想法是错误的，因为num即便不为0，也可能使得fucsym的分母为0 所以只能用try catch
    try
        value = subs(fucsym,num);
    catch
        if direct=='+'
            num = num+absSmall; %说明分母为0，除了一个0，出现错误
        elseif direct=='-'
            num = num-absSmall;
        else
            error('程序写错了，direct要不等于+ 要不等于-')
        end
        try
            value = subs(fucsym,num);
        catch
            error('程序无法处理这种程度的未定义函数值');
        end
    end
    
    if isinf(value)
        error('目前处理不了这种情况，被积函数在某一点的值都成了无穷了，函数是无界的')
    end
end