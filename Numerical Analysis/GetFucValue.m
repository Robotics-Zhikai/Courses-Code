function value = GetFucValue(fucsym,num,direct,absSmall)
    %������x ����ֵ
    %direct��ʾ��fucsym�г���һ��0ʱ���Ǹ�num��һ������С�����Ǽ�һ������С��
    %����ֻ��Ϊ��ѵ����try catch��˼�롣ʵ�������ڷ�ĸΪ0��ʱ���Ǻ�����������ھ�����������������ʱ�������򵥼Ӹ�if�ж�0����
    %û��Ҫ��try catch���������뷨�Ǵ���ģ���Ϊnum���㲻Ϊ0��Ҳ����ʹ��fucsym�ķ�ĸΪ0 ����ֻ����try catch
    try
        value = subs(fucsym,num);
    catch
        if direct=='+'
            num = num+absSmall; %˵����ĸΪ0������һ��0�����ִ���
        elseif direct=='-'
            num = num-absSmall;
        else
            error('����д���ˣ�directҪ������+ Ҫ������-')
        end
        try
            value = subs(fucsym,num);
        catch
            error('�����޷��������̶ֳȵ�δ���庯��ֵ');
        end
    end
    
    if isinf(value)
        error('Ŀǰ�������������������������ĳһ���ֵ�����������ˣ��������޽��')
    end
end