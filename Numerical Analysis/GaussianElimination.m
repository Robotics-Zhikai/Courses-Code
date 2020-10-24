function x = GaussianElimination(A,b) %��˹��ȥ�������Ax=b A�Ƿ��� b��������
    x = zeros(size(A,2),1);
    if IsSingular(A)
        error('�����ϵ������������ģ��޷��ø�˹��ȥ���������');
    end
    for i = 1:size(A,1)-1
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

