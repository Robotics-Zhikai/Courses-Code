function YES = CheckUniqueRow(Points,row)
%���Points�ĵ�row���Ƿ����ظ�ֵ ���У��򷵻�0 ���򷵻�1
    Points = SortWithRowMain(Points,row);
    for i = 1:size(Points,2)-1
        if Points(row,i)==Points(row,i+1)
            YES = 0;
            return;
        end
    end
    YES = 1;
end