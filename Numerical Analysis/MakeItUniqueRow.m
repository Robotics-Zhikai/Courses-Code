function Points = MakeItUniqueRow(Points,row)
%ʹPoints�ĵ�row��Ԫ�ر�ö���
    Points = SortWithRowMain(Points,row);
    DeleteIndex = [];
    for i=1:size(Points,2)-1
        if (Points(row,i)==Points(row,i+1))
            DeleteIndex = [DeleteIndex i];
        end
    end
    Points(:,DeleteIndex) = [];
end