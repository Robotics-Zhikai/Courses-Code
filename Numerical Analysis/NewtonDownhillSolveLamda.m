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
        IterateNum = IterateNum*2; %����ͨ��һЩ�����ж������ԣ�������äĿ��һֱ����IterateNum ���������ûʵ��
        %����ʵ�ϣ�������һЩ���������error�������ж����޷�����
    end
    
    xout = Seq(end);
end