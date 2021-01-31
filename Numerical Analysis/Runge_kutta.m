function seq = Runge_kutta(fucstr,init,h,qujian,stepnum)
%标准的龙格-库塔方法
%fucstr f(x,y) 必须变量是'x' 'y'
%init 为初值，二维行向量 (x0,y0) 
%h为求解步长
%qujian 为微分方程的解y=g(x)的x的取值区间
%stepnum为求解的步数
    seq = init;
    xn = init(1);
    yn = init(2);
    
    for i = 1:stepnum
        K1 = GetFucValue_TwoVars(fucstr,xn,yn,'+',1e-7);
        K2 = GetFucValue_TwoVars(fucstr,xn+h/2,yn+h/2*K1,'+',1e-7);
        K3 = GetFucValue_TwoVars(fucstr,xn+h/2,yn+h/2*K2,'+',1e-7);
        K4 = GetFucValue_TwoVars(fucstr,xn+h,yn+h*K3,'+',1e-7);
        xn = xn+h;
        yn = yn+h/6*(K1+2*K2+2*K3+K4);
        seq = [seq;[xn yn]];
        if xn+h>max(qujian) %肯定能够保证在取值区间内求解，但不一定能保证是stepnum步
            break;
        end
    end
end