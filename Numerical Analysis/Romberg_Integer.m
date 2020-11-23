function [result,T] = Romberg_Integer(fucstr,qujian,Error)
%��������ַ���һ�ػ��� fucstr��ʾ�����������Ա���Ϊx,qujian��ʾ��������,Error��ʾ����
%���Ϊresult�����T�� ��һ��Ϊ���ϱ�������������������������ַ��õ���ֵ
    syms x;
    fucsym = str2sym(fucstr);
    
    T_Trapezoid = [];
    T_Trapezoid(1,1) = Multi_Trapezoid_Integer(fucstr,1,qujian); 
    T_Trapezoid(1,2) = max(qujian)-min(qujian);%�ڶ��д�ŵ�ǰ�Ĳ���
    
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

function result = Recursion_Double_n(fucsym,LastqujianT,LastT,qujian)%����ʽ ��Tn�õ�T2n
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



