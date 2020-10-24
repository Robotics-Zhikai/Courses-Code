function [A,b] = TwoRowsElimination(A,b) %两行消去 其实就是把上一行的第一个未知变量代入下一行
    count = 0;
    for i = 1:size(A,2)
        if norm(A(:,i)-[0;0]) == 0
            count = count + 1;
        end
    end
    A(:,1:count) = [];
    for j=2:size(A,2)
        A(2,j) = A(2,j) -A(2,1)/A(1,1)*A(1,j);
    end
    b(2) = b(2) - A(2,1)/A(1,1)*b(1);
    A(2,1) = 0;
    A = [zeros(2,count),A];
end