function xout = SecantNewtonSolve(Func_Str,Initial_0,Initial_1,error) %���߷��󷽳̵Ľ�
%����������ʼֵ
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
        IterateNum = IterateNum*2; %����ͨ��һЩ�����ж������ԣ�������äĿ��һֱ����IterateNum ���������ûʵ��
        %����ʵ�ϣ�������һЩ���������error�������ж����޷�����
    end
    
    xout = Seq(end);
end