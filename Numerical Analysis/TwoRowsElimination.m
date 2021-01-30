function [A,b] = TwoRowsElimination(A,b) %������ȥ ��ʵ���ǰ���һ�еĵ�һ��δ֪����������һ��
    count = 0;
    record_i = -1;
    for i = 1:size(A,2)
        if norm(A(:,i)-[0;0]) == 0
            count = count + 1;
            if record_i == -1
                record_i = i;
            end
        else
            break;
        end
    end
    if record_i == 1 %���ܻ���ֱ���ǰ�ߵ�ϵ����Ϊ0�����
        A(:,1:count) = []; %ȥ��ǰ��Ϊ0��������Ա��ܹ��ظ�ʹ���������
    end
    for j=2:size(A,2)
        A(2,j) = A(2,j) -A(2,1)/A(1,1)*A(1,j);
    end
    b(2) = b(2) - A(2,1)/A(1,1)*b(1);
    A(2,1) = 0;
    
    if record_i == 1
        A = [zeros(2,count),A]; %����
    end
end