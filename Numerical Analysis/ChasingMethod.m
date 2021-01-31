function x = ChasingMethod(A,d) 
%求解三对角方程组的追赶法 Ax=d
%这类方程组在三次样条插值、数值求解常微分方程边值问题及偏微分方程有限差分方法中经常遇到
%本质上还是个高斯消元法
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
    
    absa = abs(a);
    absb = abs(b);
    absc = abs(c);
    
    if size(find(absa==0),2)>1 || size(find(absc==0),2)>1 || (~(absb(1)>absc(1) && absc(1)>0)) || (~(absb(end)>absa(end) && absa(end)>0))
        error('不满足使用追赶法的条件');
    end
    
    for i=2:dimA-1
        if ~(absb(i)>=absa(i)+absc(i))
            error('不满足使用追赶法的条件');
        end
    end
    
    if size(find(A(1,:)~=0),2)~=2 || size(find(A(end,:)~=0),2)~=2 %至此处，已经保证了所有的a b c除了设定为0之外的值均不为0
        error('不满足使用追赶法的条件');
    end
    for i = 2:dimA-1
        if size(find(A(i,:)~=0),2)~=3
            error('不满足使用追赶法的条件');
        end
    end
    %至此不出错说明可以用追赶法求解三对角方程组
    
    beita = c(1)/b(1);
    for i = 2:dimA-1
        beita = [beita c(i)/(b(i)-a(i)*beita(end))];
    end
    
    y = d(1)/b(1);
    for i = 2:dimA
        y = [y (d(i)-a(i)*y(end))/(b(i)-a(i)*beita(i-1))];
    end
    
    x = zeros(dimA,1);
    x(end) = y(end);
    for i = dimA-1:-1:1
        x(i) = y(i)-beita(i)*x(i+1);
    end
    
end
















