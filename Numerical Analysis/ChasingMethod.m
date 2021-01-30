function x = ChasingMethod(A,d) 
%������ԽǷ������׷�Ϸ� Ax=d
%���෽����������������ֵ����ֵ��ⳣ΢�ַ��̱�ֵ���⼰ƫ΢�ַ������޲�ַ����о�������
%�����ϻ��Ǹ���˹��Ԫ��
    if size(A,1)~=size(A,2)
        error('���޷�����������');
    end
    %�����ж��Ƿ�����׷�Ϸ�������
    dimA = size(A,1);
    
    b = [];
    for i=1:dimA
        b = [b A(i,i)];
    end
    
    c = [];
    for i = 1:dimA-1
        c = [c A(i,i+1)];
    end
    c = [c 0];
    
    a = 0;
    for i = 2:dimA
        a = [a A(i,i-1)];
    end
    
    absa = abs(a);
    absb = abs(b);
    absc = abs(c);
    
    if size(find(absa==0),2)>1 || size(find(absc==0),2)>1 || (~(absb(1)>absc(1) && absc(1)>0)) || (~(absb(end)>absa(end) && absa(end)>0))
        error('������ʹ��׷�Ϸ�������');
    end
    
    for i=2:dimA-1
        if ~(absb(i)>=absa(i)+absc(i))
            error('������ʹ��׷�Ϸ�������');
        end
    end
    
    if size(find(A(1,:)~=0),2)~=2 || size(find(A(end,:)~=0),2)~=2 %���˴����Ѿ���֤�����е�a b c�����趨Ϊ0֮���ֵ����Ϊ0
        error('������ʹ��׷�Ϸ�������');
    end
    for i = 2:dimA-1
        if size(find(A(i,:)~=0),2)~=3
            error('������ʹ��׷�Ϸ�������');
        end
    end
    %���˲�����˵��������׷�Ϸ�������ԽǷ�����
    
    beita = c(1)/b(1);
    for i = 2:dimA-1
        beita = [beita c(i)/(b(i)-a(i)*beita(end))];
    end
    
    y = d(1)/b(1);
    for i = 2:dimA
        y = [y (d(i)-a(i)*y(end))/(b(i)-a(i)*beita(i-1))];
    end
    
    x = zeros(dimA,1);
    x(end) = y(end);
    for i = dimA-1:-1:1
        x(i) = y(i)-beita(i)*x(i+1);
    end
    
end
















