function xout = NewtonSolve(Func_str,initialValue,error)
%����Ϊfx 
    syms x
    initFunc_str = str2sym(Func_str);
    Func_str = str2sym(Func_str);
    Func_str = x - Func_str/diff(Func_str,1);
    
    IterateNum = 10;
    
    while 1
        Seq = IteratedFunc(Func_str,initialValue,IterateNum);
        if abs(subs(initFunc_str,Seq(end)))<error
            break;
        end
        IterateNum = IterateNum*2; %����ͨ��һЩ�����ж������ԣ�������äĿ��һֱ����IterateNum ���������ûʵ��
        %����ʵ�ϣ�������һЩ���������error�������ж����޷�����
    end
    
    xout = Seq(end);
end