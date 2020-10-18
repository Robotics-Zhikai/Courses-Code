function xout = SecantNewtonSolve(Func_Str,Initial_0,Initial_1,error) %割线法求方程的解
%输入两个初始值
    syms x0 x1
    syms x
    initFunc_str = str2sym(Func_Str);
    Func_Str = str2sym(Func_Str);
    InterateFunc = x1 - ( subs(Func_Str,x1)/(subs(Func_Str,x1)-subs(Func_Str,x0)) )*(x1-x0);
    
    IterateNum = 10;
    while 1
        Seq = IteratedFunc(InterateFunc,[Initial_0,Initial_1],IterateNum,error);
        if abs(subs(initFunc_str,Seq(end)))<error
            break;
        end
        IterateNum = IterateNum*2; %可以通过一些方法判断收敛性，而不必盲目的一直增大IterateNum 这个函数还没实现
        %而事实上，可能在一些迭代后出现error，即可判断是无法收敛
    end
    
    xout = Seq(end);
end