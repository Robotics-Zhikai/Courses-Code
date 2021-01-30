function YES = IsSingular(A)
    YES = 0;
    A = abs(A);
    for i = 1:size(A,1)
        if max(A(i,:))==0
            YES = 1;
        end
    end
end