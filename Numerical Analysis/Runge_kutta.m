function seq = Runge_kutta(fucstr,init,h,qujian,stepnum)
%��׼������-��������
%fucstr f(x,y) ���������'x' 'y'
%init Ϊ��ֵ����ά������ (x0,y0) 
%hΪ��ⲽ��
%qujian Ϊ΢�ַ��̵Ľ�y=g(x)��x��ȡֵ����
%stepnumΪ���Ĳ���
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
        if xn+h>max(qujian) %�϶��ܹ���֤��ȡֵ��������⣬����һ���ܱ�֤��stepnum��
            break;
        end
    end
end