function x = ChasingMethod(A,b) 
%������ԽǷ������׷�Ϸ� Ax=b
%���෽����������������ֵ����ֵ��ⳣ΢�ַ��̱�ֵ���⼰ƫ΢�ַ������޲�ַ����о�������
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
    
    if size(find(a==0),2)>1 || size(find(c==0),2)>1
        error('������ʹ��׷�Ϸ�������');
    end
end
















