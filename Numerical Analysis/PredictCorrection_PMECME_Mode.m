function seq = PredictCorrection_PMECME_Mode(fucstr,init,h,qujian,stepnum)
%用PMECME模式的预测矫正法解常微分方程
%fucstr f(x,y) 必须变量是'x' 'y'
%init 为初值，二维行向量 (x0,y0) 
%h为求解步长
%qujian 为微分方程的解y=g(x)的x的取值区间
%stepnum为求解的步数
    if stepnum <= 3 && stepnum>=0
        seq = Runge_kutta(fucstr,init,h,qujian,stepnum);
        return ;
    end
    seq = Runge_kutta(fucstr,init,h,qujian,3); %先用相对来说不那么精确的方法来获得四个启动值
    
    c = 0; p = 0;
    f = [];
    f(1,1) = GetFucValue_TwoVars(fucstr,seq(1,1),seq(1,2),'+',1e-7);
    f(2,1) = GetFucValue_TwoVars(fucstr,seq(2,1),seq(2,2),'+',1e-7);
    f(3,1) = GetFucValue_TwoVars(fucstr,seq(3,1),seq(3,2),'+',1e-7);
    for currentn = 4:stepnum
        fn = GetFucValue_TwoVars(fucstr,seq(currentn,1),seq(currentn,2),'+',1e-7);
        fn_1 = f(end);
        fn_2 = f(end-1);
        fn_3 = f(end-2);
        f = [f;fn];
        yn = seq(currentn,2);
        p_np1 = yn+h/24*(55*fn-59*fn_1+37*fn_2-9*fn_3);
        m_np1 = p_np1 + 251/270*(c-p);
        x_np1 = seq(currentn,1)+h;
        d_m_np1 = GetFucValue_TwoVars(fucstr,x_np1,m_np1,'+',1e-7);
        c_np1 = yn + h/24*(9*d_m_np1+19*fn-5*fn_1+fn_2);
        y_np1 = c_np1-19/270*(c_np1-p_np1);
%         d_y_np1 = GetFucValue_TwoVars(fucstr,x_np1,y_np1,'+',1e-7);
%         %这个没什么用
        xn = x_np1;
        yn = y_np1;
        seq = [seq;[xn,yn]];
        if xn+h>max(qujian)
            break;
        end
    end
end










