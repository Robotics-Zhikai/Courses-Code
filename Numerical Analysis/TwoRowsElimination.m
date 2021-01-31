function [A,b] = TwoRowsElimination(A,b) %两行消去 其实就是把上一行的第一个未知变量代入下一行
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
    if record_i == 1 %可能会出现本来前边的系数就为0的情况
        A(:,1:count) = []; %去掉前边为0的情况，以便能够重复使用这个函数
    end
    for j=2:size(A,2)
        A(2,j) = A(2,j) -A(2,1)/A(1,1)*A(1,j);
    end
    b(2) = b(2) - A(2,1)/A(1,1)*b(1);
    A(2,1) = 0;
    
    if record_i == 1
        A = [zeros(2,count),A]; %补足
    end
end