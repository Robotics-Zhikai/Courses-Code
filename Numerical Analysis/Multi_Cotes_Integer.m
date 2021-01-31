function Result = Multi_Cotes_Integer(fucstr,n,qujian) 
%����Cotes�����
%fucstrΪ��������
%nΪ���ֵ�����������
%qujianΪ1*2ά������qujian(1)Ϊ������߽� qujian(2)Ϊ�����ұ߽�
    if n<=0
        error('���ֵ�����������С�ڻ����0');
    end
    syms x;
    fucsym = str2sym(fucstr);
    h = (max(qujian)-min(qujian))/n;
    sum = 0;
    leftupper = min(qujian);
    rightupper = max(qujian);
    absSmall = 0.1*1e-5;
    for i = 0:n-1
        sum = sum + 7*GetFucValue(fucsym,leftupper+i*h,'+',absSmall) + 7*GetFucValue(fucsym,leftupper+(i+1)*h,'-',absSmall) + ...
            12*GetFucValue(fucsym,leftupper+(i+0.5)*h,'-',absSmall) + 32*GetFucValue(fucsym,leftupper+(i+1/4)*h,'-',absSmall)+...
            32*GetFucValue(fucsym,leftupper+(i+3/4)*h,'-',absSmall);
    end
    Result = h*sum/90;
end