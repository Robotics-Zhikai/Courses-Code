function x = ChasingMethod(A,b) 
%求解三对角方程组的追赶法 Ax=b
%这类方程组在三次样条插值、数值求解常微分方程边值问题及偏微分方程有限差分方法中经常遇到
    if size(A,1)~=size(A,2)
        error('暂无法求解此类问题');
    end
    %首先判断是否满足追赶法的条件
    dimA = size(A,1);
    
    b = [];
    for i=1:dimA
        b = [b A(i,i)];
    end
    
    c = [];
    for i = 1:dimA-1
        c = [c A(i,i+1)];
    end
    c = [c 0];
    
    a = 0;
    for i = 2:dimA
        a = [a A(i,i-1)];
    end
    
    if size(find(a==0),2)>1 || size(find(c==0),2)>1
        error('不满足使用追赶法的条件');
    end
end
















