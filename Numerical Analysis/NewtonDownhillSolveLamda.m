function xout = NewtonDownhillSolveLamda(Func_str,initvalue,lamda,error)
    syms x
    initFunc_str = str2sym(Func_str);
    Func_str = str2sym(Func_str);
    Func_str = x - lamda*Func_str/diff(Func_str,1);
    
    IterateNum = 10;
    while 1
        Seq = IteratedFunc(Func_str,initvalue,IterateNum,error);
        if abs(subs(initFunc_str,Seq(end)))<error
            break;
        end
        IterateNum = IterateNum*2; %可以通过一些方法判断收敛性，而不必盲目的一直增大IterateNum 这个函数还没实现
        %而事实上，可能在一些迭代后出现error，即可判断是无法收敛
    end
    
    xout = Seq(end);
end