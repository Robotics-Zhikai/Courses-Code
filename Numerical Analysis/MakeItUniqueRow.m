function Points = MakeItUniqueRow(Points,row)
%使Points的第row行元素变得独特
    Points = SortWithRowMain(Points,row);
    DeleteIndex = [];
    for i=1:size(Points,2)-1
        if (Points(row,i)==Points(row,i+1))
            DeleteIndex = [DeleteIndex i];
        end
    end
    Points(:,DeleteIndex) = [];
end