function [result,T] = Romberg_Integer(fucstr,qujian,Error)
%龙贝格积分法，一重积分 fucstr表示被积函数，自变量为x,qujian表示被积区间,Error表示精度
%输出为result结果和T表 第一列为不断倍增复化区间数量的梯形求积分法得到的值
    syms x;
    fucsym = str2sym(fucstr);
    
    T_Trapezoid = [];
    T_Trapezoid(1,1) = Multi_Trapezoid_Integer(fucstr,1,qujian); 
    T_Trapezoid(1,2) = max(qujian)-min(qujian);%第二列存放当前的步长
    
    T = T_Trapezoid(1,1);
    for i = 2:200
        T_Trapezoid(i,1) = Recursion_Double_n(fucsym,T_Trapezoid(i-1,2),T_Trapezoid(i-1,1),qujian);
        T_Trapezoid(i,2) = T_Trapezoid(i-1,2)/2;
        T(i,1) = T_Trapezoid(i,1);
        m = 2;
        Rowsize = i;
        for j = i:-1:2
            T(Rowsize-1,m)=(4^(m-1)*T(Rowsize,m-1)-T(Rowsize-1,m-1))/(4^(m-1)-1);
            m=m+1;
            Rowsize = Rowsize - 1;
        end
        if abs(T(1,end)-T(1,end-1))<Error
            break;
        end
    end
   
    result = T(1,end);
end

function result = Recursion_Double_n(fucsym,LastqujianT,LastT,qujian)%递推式 由Tn得到T2n
    h = LastqujianT;
    sum = 0;
    current = min(qujian)+h/2;
    while(current<max(qujian)-(h/20))
        sum = sum + GetFucValue(fucsym,current,'-',0.1*1e-6);
        current = current + h;
    end
    U = h * sum;    
    result = 1/2*(LastT+U);
end



