function Result = Multi_Trapezoid_Integer(fucstr,n,qujian) 
%复化梯形求积分
%fucstr为被积函数
%n为划分的子区间数量
%qujian为1*2维向量，qujian(1)为积分左边界 qujian(2)为积分右边界
    if n<=0
        error('划分的区间数不能小于或等于0');
    end
    syms x;
    fucsym = str2sym(fucstr);
    h = (max(qujian)-min(qujian))/n;
    sum = 0;
    leftupper = min(qujian);
    rightupper = max(qujian);
    absSmall = 0.1*1e-5;
    for i = 0:n-1
        sum = sum + GetFucValue(fucsym,leftupper+i*h,'+',absSmall) + GetFucValue(fucsym,leftupper+(i+1)*h,'-',absSmall);
    end
    Result = h*sum/2;
end