function x = GaussianColumnElimination(A,b) %高斯列主元消去法，求解Ax=b A是方阵 b是列向量 处理具有极小量元素的系数矩阵时采用
    x = zeros(size(A,2),1);
    if IsSingular(A)
        error('输入的系数矩阵是奇异的，无法用高斯消去法进行求解');
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






