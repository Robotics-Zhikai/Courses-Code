function value = GetFucValue_TwoVars(fucstr,x,y,direct,absSmall)
    %˫����x y����ֵ
    %direct��ʾ����fucsym����һ��0ʱ���Ǹ�x y ��һ������С�����Ǽ�һ������С��
    %fucstr = f(x,y) ��string����
    try
        value = eval(fucstr); %��Ϊ�Ƕ���� ����ֻ����eval str
    catch
        if direct=='+'
            x = x+absSmall; %˵����ĸΪ0������һ��0�����ִ���
            y = y+absSmall;
        elseif direct=='-'
            x = x-absSmall;
            y = y-absSmall;
        else
            error('����д���ˣ�directҪ������+ Ҫ������-')
        end
        try
            value = eval(fucstr);
        catch
            error('�����޷��������̶ֳȵ�δ���庯��ֵ');
        end
    end
    
    if isinf(value)
        error('Ŀǰ�������������������������ĳһ���ֵ�����������ˣ��������޽��')
    end
end