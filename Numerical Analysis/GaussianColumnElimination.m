function x = GaussianColumnElimination(A,b) %��˹����Ԫ��ȥ�������Ax=b A�Ƿ��� b�������� ������м�С��Ԫ�ص�ϵ������ʱ����
    x = zeros(size(A,2),1);
    if IsSingular(A)
        error('�����ϵ������������ģ��޷��ø�˹��ȥ���������');
    end
    for i = 1:size(A,1)-1
        [A,b] = ExchangeSmall(A,b,i,i);
        for j = i+1:size(A,1)
            tmpA = [A(i,:);A(j,:)];
            tmpb = [b(i);b(j)];
            [tmpA,tmpb] = TwoRowsElimination(tmpA,tmpb);
            A(j,:) = tmpA(2,:);
            b(j) = tmpb(2);
        end
    end
    N = size(A,2);
    x(N) = b(N)/A(N,N);
    for k = N-1:-1:1
        Sum = 0;
        for j=k+1:N
            Sum = Sum + A(k,j)*x(j);
        end
        x(k) = (b(k)-Sum)/A(k,k);
    end
end

function [A,b] = ExchangeSmall(A,b,row,lie)
    Atmp = abs(A);
%     Atmp = Atmp(row:end,lie:end);
    for i = row:size(Atmp,1)
        if Atmp(i,lie) == max(Atmp(row:end,lie))
            mid = A(row,:);
            A(row,:) = A(i,:);
            A(i,:) = mid;
            
            mid = b(row);
            b(row) = b(i);
            b(i) = mid;

%             r1 = row;
%             r2 = i;
            break;
        end
    end
end






