function x = GaussianElimination(A,b) %高斯消去法，求解Ax=b A是方阵 b是列向量
    x = zeros(size(A,2),1);
    if IsSingular(A)
        error('输入的系数矩阵是奇异的，无法用高斯消去法进行求解');
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

