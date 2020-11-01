function YES = CheckUniqueRow(Points,row)
%检查Points的第row行是否有重复值 如有，则返回0 否则返回1
    Points = SortWithRowMain(Points,row);
    for i = 1:size(Points,2)-1
        if Points(row,i)==Points(row,i+1)
            YES = 0;
            return;
        end
    end
    YES = 1;
end